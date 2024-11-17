use std::{collections::VecDeque, sync::Mutex, thread::{self, park}};

use super::Queue;

//TODO: "Possibility to specify the amount of time to lock"
impl<T> super::queue::Queue<T> {
	pub fn new(capacity: usize) -> Queue<T> {
		let data = Mutex::new(VecDeque::with_capacity(capacity));
		let waiting = Mutex::new(Vec::new());

		return Queue { data, waiting };
	}

	pub fn push(&self, value: T)
	{
		let guarded_data = self.data.lock();

		match guarded_data {
			Ok(mut data) => data.push_back(value),
			Err(_)=> ()
		}
	}

	fn maybe_data(&self) -> Option<T> {
		let mut r = Option::None;
		let guarded_data = self.data.lock();
	
		match guarded_data {
			Ok(mut data) => {
				
				if !data.is_empty() {
					r.insert(data.pop_front().unwrap());
				}
			},
			Err(_) => (),
		}

		return r;
	}

	fn make_waiting(&self) {
		let guarded_wait = self.waiting.lock();

		match guarded_wait {
			Ok(mut wait) => wait.push(thread::current().id()),
			Err(_) => ()
		}
	}

	//TODO: This should always return a value and wait if the queue is empty
	pub fn get(&self) -> T {
		let mut r = Option::None;

		loop {
			r = self.maybe_data();

			if r.is_none() {
				self.make_waiting();
				//TODO: Investigate park_timeout()
				park();
			}
			else {
				break;
			}
		}

		return r.unwrap();
	}
}