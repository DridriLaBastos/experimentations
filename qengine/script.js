import init , { simulate } from './quantum-engine/pkg/quantum_engine.js';

const ASPECT_RATIO = 16/9;
const DISP_WIDTH = 400;
const DISP_HEIGHT = DISP_WIDTH / ASPECT_RATIO;

const SIM_RATIO = 3;

const SIM_WIDTH = DISP_WIDTH / SIM_RATIO;
const SIM_HEIGHT = DISP_HEIGHT / SIM_RATIO;

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const simulation = new Float32Array(SIM_WIDTH * SIM_HEIGHT);

canvas.setAttribute('width', DISP_WIDTH);
canvas.setAttribute('height', DISP_HEIGHT);
canvas.style.borderRadius = '8px';
canvas.style.border = '2px solid black';

init().then(() => {
	simulate(simulation, SIM_WIDTH, SIM_HEIGHT);

	const imageData = ctx.createImageData(SIM_WIDTH, SIM_HEIGHT);

	for (let i = 0; i < simulation.length; i++) {
		const pixel = simulation[i] * 255;
		imageData.data[i * 4] = pixel;
		imageData.data[i * 4 + 1] = pixel;
		imageData.data[i * 4 + 2] = pixel;
		imageData.data[i * 4 + 3] = 255;
	}

	ctx.putImageData(imageData, 0, 0);
	ctx.drawImage(canvas, 0, 0, SIM_WIDTH, SIM_HEIGHT, 0, 0, DISP_WIDTH, DISP_HEIGHT);
});
