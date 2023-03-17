const { SerialPort, SerialPortMock } = require("serialport")

SerialPort.list().then(ports => {
    ports.forEach(port => {
        console.log(port);
    })
})
const port = new SerialPort({
    baudRate: 115200,
    path: "EqualizerAPOUI"
})

function run() {
    port.open(err => {
        if (!err) {
            port.write("espeq");
            port.once('data', data => {
                if (data.toString() === "ok") {
                    console.log("Device connected");
                    port.on('data', data => {
                        console.log(parseInt(data.toString("hex"), 16))
                    })
                }
            })
        } else console.log
    });

}

module.exports.run = run;