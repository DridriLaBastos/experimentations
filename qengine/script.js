import init , { add } from './quantum-engine/pkg/quantum_engine.js';

const ASPECT_RATIO = 16/9;
const SIM_WIDTH = 400;
const SIM_HEIGHT = SIM_WIDTH / ASPECT_RATIO;

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');

canvas.setAttribute('width', SIM_WIDTH);
canvas.setAttribute('height', SIM_HEIGHT);
canvas.style.borderRadius = '8px';
canvas.style.border = '2px solid black';

init().then(() => {
	console.log("Wasm module loaded");
	console.log(add(1, 2));
});
