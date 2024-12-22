use wasm_bindgen::prelude::*;

#[wasm_bindgen]
pub fn simulate(buffer: &mut [f32], width: u32, height: u32) 
{
	for i in 0..((width*height) as usize) {
		buffer[i] = i as f32 / ((width*height) as f32);
	}
}
