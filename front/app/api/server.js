const express = require('express');
const { WebSocketServer } = require('ws');
const bodyParser = require('body-parser');

const WS_PORT = 4443;

const app = express();
app.use(bodyParser.json());

// WebSocket 서버 생성
const wss = new WebSocketServer({ noServer: true });

wss.on('connection', (ws) => {
    ws.on('message', (message) => {
        console.log(`Received: ${message}`);

        // 모든 클라이언트에게 메시지 전송 (broadcast)
        wss.clients.forEach(client => {
            if (client.readyState === 1) {
                client.send(message);
            }
        });
    });
});

// HTTP 서버에서 WebSocket을 처리
const server = app.listen(WS_PORT, () => {
    console.log(`Server running on http://165.132.142.126:${WS_PORT}`);
});

server.on('upgrade', (request, socket, head) => {
    wss.handleUpgrade(request, socket, head, (ws) => {
        wss.emit('connection', ws, request);
    });
});

// HTTP POST 요청을 WebSocket 메시지로 변환
app.post('/api/ws', (req, res) => {
    const { type, message } = req.body;

    if (!type || !message) {
        return res.status(400).json({ error: 'Invalid request format' });
    }

    // WebSocket 클라이언트들에게 메시지 전송
    wss.clients.forEach(client => {
        if (client.readyState === 1) {
            client.send(JSON.stringify({ type, message }));
        }
    });

    res.json({ success: true, message: 'Message sent to WebSocket clients' });
});