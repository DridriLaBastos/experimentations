
pub struct Queue<T> {
	data: std::collections::VecDeque<T>,
	data_mutex: std::sync::Mutex<i32>,
	sync_mutex: std::sync::Mutex<i32>,
	waiters: std::collections::VecDeque<std::thread::Thread>,
}

impl<T> Queue<T> {
	pub fn new (size: usize) -> Queue<T> {
		let data = std::collections::VecDeque::with_capacity(size);
		let data_mutex = std::sync::Mutex::new(0);
		let sync_mutex = std::sync::Mutex::new(0);
		let waiters = std::collections::VecDeque::new();
		Queue { data, data_mutex, sync_mutex, waiters }
	}

	//TODO: How to notify if we can't get the lock ?
	pub fn push(&mut self, data: T) {
		let data_lock = self.data_mutex.try_lock();

		if let Ok(_) = data_lock {
			self.data.push_front(data);
		}

		let sync_lock = self.sync_mutex.try_lock();

		if let Ok(_) = sync_lock {
			if !self.waiters.is_empty() {
				let last = self.waiters.pop_back().unwrap();
				last.unpark();
			}
		}

	}
	
	pub fn get(&mut self) -> T {
		let data_lock = self.data_mutex.try_lock();
		let mut data: T;
		if let Ok(_) = data_lock {
			if ! self.data.is_empty() {
				data = self.data.pop_back().unwrap();
			}
		}

		let sync_lock = self.sync_mutex.try_lock();
		if let Ok(_) = sync_lock {
			self.waiters.push_back(std::thread::current());
		}

		std::thread::park();

		data = self.data.pop_back().unwrap();

		return data;
	}
}
