use std::{collections::VecDeque, sync::Mutex, thread::{self, current, park}};

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

		let guarded_waiter = self.waiting.lock();

		match guarded_waiter {
			Ok(mut wait) => {
				let maybe_thread = wait.pop();

				match maybe_thread {
					Some(thread) => thread.unpark(),
					None => (),
				}
			},
			Err(_) => ()
		}
	}

	fn make_waiting(&self) {
		let guarded_wait = self.waiting.lock();

		match guarded_wait {
			Ok(mut wait) => {
				wait.push(thread::current());
				//There might be a bug here if unpark is called for this thread from the push function before the call to
				//park happen.
				std::mem::drop(wait);
				park();
			},
			Err(_) => panic!("Error while acquiring lock")
		}
	}

	fn maybe_data(&self) -> Option<T> {
		let guarded_data = self.data.lock();
		let mut r = Option::None;
	
		match guarded_data {
			Ok(mut data) => {
				r = data.pop_back();
			},
			Err(_) => (),
		}

		return r;
	}

	//TODO: This should always return a value and wait if the queue is empty
	pub fn get(&self) -> T {
		let mut r = self.maybe_data();

		if r.is_none() {
			self.make_waiting();
			//here there is an error if r is None
			r = self.maybe_data();
		}

		return r.unwrap();
	}
}