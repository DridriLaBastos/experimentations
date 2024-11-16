use std::{collections::VecDeque, sync::Mutex};

use super::Queue;

//TODO: "Possibility to specify the amount of time to lock"
impl<T> super::queue::Queue<T> {
	pub fn new(capacity: usize) -> Queue<T> {
		let data = Mutex::new(VecDeque::with_capacity(capacity));

		return Queue { data }
	}

	pub fn push(&mut self, value: T)
	{
		let guarded_data = self.data.lock();

		match guarded_data {
			Ok(mut data) => data.push_back(value),
			Err(_)=> ()
		}
	}

	//TODO: This should always return a value and wait if the queue is empty
	pub fn get(&mut self) -> Option<T> {
		let guarded_data = self.data.lock();
		let r = match guarded_data {
			Ok(mut data) => data.pop_front(),
			Err(_) => Option::None
		};

		return r;
	}
}