const BLIND_NUM = 6
const height = 320
const roof_height = 40

var parameter_list = {}
const isDark = window.matchMedia("(prefers-color-scheme: dark)").matches        // Get Appearance
const width = window.innerWidth
const middle_x = Math.round(width / 2)
const BLIND_START = {x: middle_x - 0, y: 140, start_angle: 10}

// Reset body width
document.body.style.width = width + "px"

// Setup Canvas
const canvas = document.createElement("canvas");
canvas.width = width;
canvas.height = height;
canvas.id = "myCanvas";
document.body.appendChild(canvas);
const ctx = canvas.getContext("2d");

function render_default() {
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
}

function render_blinds() {
    const blind_half_length = (height - BLIND_START.y) / BLIND_NUM / 2;
    const angle = Math.PI * (parameter_list["angle"] + BLIND_START.start_angle - 90) / 180
    const x = BLIND_START.x;
    ctx.lineWidth = 2 ;
    ctx.strokeStyle = isDark ? "white" : "#000000";
    ctx.beginPath();
    ctx.moveTo(width, BLIND_START.y - roof_height);
    ctx.lineTo(x, BLIND_START.y - roof_height);
    ctx.lineTo(x, BLIND_START.y);
    for (let index = 0; index < BLIND_NUM; index++) {
        const y = BLIND_START.y + blind_half_length * (2 * index + 1);
        ctx.moveTo(x + blind_half_length * Math.cos(angle), y - blind_half_length * Math.sin(angle));
        ctx.lineTo(x - blind_half_length * Math.cos(angle), y + blind_half_length * Math.sin(angle));
    }
    ctx.stroke();
    //
}

function render() {
    try {
        if (parameter_list["angle"]) {
            if (Number.isFinite( +parameter_list["angle"] )) {
                parameter_list["angle"] *= 1
                render_blinds()
            } else {
                throw new Error("Parameter 'angle' is not a number");
            }
        } else {
            throw new Error("Parameter 'angle' not found");
        }
    } catch (error) {
        console.log(error);
        document.body.innerHTML = "Waiting for ESP32 to respond";
    }
}

function get_data() {
    const parameter_str_list = location.search.split("?").at(-1).split("&")
    for (var parameter_str of parameter_str_list) {
        parameter_str = parameter_str.split("=");
        if (parameter_str.length = 2) {
            parameter_list[parameter_str[0]] = parameter_str[1]
        }
    }
    console.log(parameter_list);
    
}

function setup() {
    get_data()
    render()
}

setup()