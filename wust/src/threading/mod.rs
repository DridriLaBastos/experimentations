use std::{collections::VecDeque, sync::Mutex};

pub mod queue;

pub struct Queue<T> {
	data: Mutex<VecDeque<T>>
}