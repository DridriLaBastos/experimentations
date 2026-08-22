import {defineConfig} from 'vite';
import motionCanvas from '@motion-canvas/vite-plugin';
import ffmpeg from '@motion-canvas/ffmpeg';

export default defineConfig({
  server: {
    host: '0.0.0.0',
    port: 9000,
  },
  plugins: [
    motionCanvas(),
    ffmpeg(),
  ],
});
