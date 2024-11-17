use std::{collections::VecDeque, sync::Mutex, thread::Thread};

pub mod queue;

pub struct Queue<T> {
	data: Mutex<VecDeque<T>>,
	waiting: Mutex<Vec<Thread>>
}
