const { app } = require('electron');
let dgram = require('node:dgram');
const main = require('.');
app.on('before-quit', e => {
    socket.send('leaving', 1234, '192.168.1.254');
})
const socket = dgram.createSocket('udp4');
socket.on('error', (err) => {
    console.error(`server error:\n${err.stack}`);
    socket.close();
});
socket.bind(1234);
socket.on('listening', () => {
    const address = socket.address();
    console.log(`server listening ${address.address}:${address.port}`);
});
socket.on('connect', () => {
    console.log('connected');
})
socket.connect(1234, '192.168.1.254', () => {
    console.log('connected');
    socket.send('init');
});

module.exports.socket = socket;