"""
FastAPI backend for the live IoT sensor dashboard.

Architecture:
    MQTT subscriber (separate script) -> SQLite -> [this app polls SQLite]
    -> broadcasts new rows over WebSocket -> browser (Chart.js)

ADJUST THESE to match your actual setup:
    - DB_PATH: path to your SQLite file
    - TABLE / COLUMNS in get_new_rows(): match your subscriber's schema
    - POLL_INTERVAL: how often (seconds) to check SQLite for new rows
"""

import asyncio
import json
import sqlite3
from pathlib import Path

from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.staticfiles import StaticFiles
from fastapi.responses import FileResponse

# ---- CONFIG: adjust to match your project -------------------------------
DB_PATH = r"C:\Users\EKRco\Desktop\data.db"          # <-- path to your existing SQLite file
TABLE = "data"              # <-- your table name
POLL_INTERVAL = 2.0             # seconds between SQLite checks
# ---------------------------------------------------------------------------

app = FastAPI()
app.mount("/static", StaticFiles(directory="static"), name="static")

# Keeps track of every browser currently connected via WebSocket
connected_clients: set[WebSocket] = set()

# Remembers the last row id we already sent, so we only fetch new ones
last_sent_id = 0


def get_new_rows(since_id: int):
    """Fetch rows from SQLite with id greater than since_id."""
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    cur = conn.cursor()
    cur.execute(
        f"SELECT * FROM {TABLE} WHERE id > ? ORDER BY id ASC",
        (since_id,),
    )
    rows = [dict(row) for row in cur.fetchall()]
    conn.close()
    return rows


async def poll_and_broadcast():
    """Background loop: check SQLite for new rows, push them to all clients."""
    global last_sent_id
    while True:
        try:
            new_rows = get_new_rows(last_sent_id)
            if new_rows:
                last_sent_id = new_rows[-1]["id"]
                message = json.dumps(new_rows)
                # send to every connected browser; drop any that failed
                dead_clients = set()
                for client in connected_clients:
                    try:
                        await client.send_text(message)
                    except Exception:
                        dead_clients.add(client)
                connected_clients.difference_update(dead_clients)
        except Exception as e:
            print(f"[poll_and_broadcast] error: {e}")

        await asyncio.sleep(POLL_INTERVAL)


@app.on_event("startup")
async def startup_event():
    # Initialize last_sent_id to the current max id so we only stream
    # NEW data from now on, not the entire history.
    global last_sent_id
    try:
        conn = sqlite3.connect(DB_PATH)
        cur = conn.cursor()
        cur.execute(f"SELECT MAX(id) FROM {TABLE}")
        result = cur.fetchone()[0]
        last_sent_id = result if result else 0
        conn.close()
    except Exception as e:
        print(f"[startup] could not read initial max id: {e}")

    asyncio.create_task(poll_and_broadcast())


@app.get("/")
async def serve_index():
    return FileResponse("static/index.html")


@app.get("/api/history")
async def get_history(limit: int = 100):
    """Returns the most recent `limit` rows, oldest first — used to
    pre-populate the chart when a browser first loads the page."""
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    cur = conn.cursor()
    cur.execute(f"SELECT * FROM {TABLE} ORDER BY id DESC LIMIT ?", (limit,))
    rows = [dict(row) for row in cur.fetchall()]
    conn.close()
    rows.reverse()  # oldest first, matches how the chart expects data
    return rows


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    connected_clients.add(websocket)
    print(f"Client connected. Total clients: {len(connected_clients)}")
    try:
        while True:
            # We don't expect messages from the browser, but this keeps
            # the connection alive and lets us detect disconnects.
            await websocket.receive_text()
    except WebSocketDisconnect:
        connected_clients.discard(websocket)
        print(f"Client disconnected. Total clients: {len(connected_clients)}")


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
