use std::{io::{BufRead, BufReader, Write}, net::{TcpListener, TcpStream}, usize};

mod threading;

struct Connection {
	connection: TcpStream,
	http_request: Vec<String>,
}

fn handle_request_thread_function(queue: std::sync::Arc<threading::Queue<Connection>>)
{
	loop {
		let mut entry = queue.get();
		
		log::debug!("*** New Request ***");

		for i in 0 .. entry.http_request.len()
		{
			let e = &entry.http_request[i];
			log::trace!("{e}");
		}

		let start_line_split = entry.http_request[0].split_whitespace().collect::<Vec<&str>>();

		match start_line_split[0] {
			"GET" => log::info!("GET request"),
			"POST" => log::info!("POST request"),
			_ => log::info!("Unknown request"),
		}

		entry.connection.write_all(b"HTTP/1.1 200 OK\r\n\r\n");
	}
}

//TODO: To be quick I use unwrap, but those cases should be handled properly
fn handle_data_received(stream: TcpStream, queue: &std::sync::Arc<threading::Queue<Connection>>)
{
	let buf_reader = BufReader::new(&stream);
	let mut http_request = Vec::new();

	let connection_addr = stream.peer_addr().unwrap();
	log::info!("Got Connection from {}.{}",connection_addr.ip(),connection_addr.port());

	for maybe_line in buf_reader.lines() {
		let line = maybe_line.unwrap();
		if line.is_empty() {
			break;
		} else {
			http_request.push(line);
		}
	}

	queue.push(Connection { connection: (stream), http_request: (http_request) });
}

fn server_loop(listener: TcpListener, queue: std::sync::Arc<threading::Queue<Connection>>)
{
	let addr = listener.local_addr().unwrap();
	println!("Listening from {addr}");
	for stream in listener.incoming()
	{
		match stream
		{
			Ok(s) => handle_data_received(s, &queue),
			Err(e) => panic!("Unable to create a connection : {e:?}"),
		}
	}
}

//IMPORTANT: For the log messages to be displayed, the RUST_LOG environment variable must set
fn main() {
	let did_listen = TcpListener::bind("127.0.0.1:7878");
	let thread_number: usize = 2;
	let queue = std::sync::Arc::new(threading::Queue::new(5));
	let mut handles = std::vec::Vec::new();

	env_logger::init();

	for _ in 0..thread_number {
		let new_queue = std::sync::Arc::clone(&queue);
		handles.push(std::thread::spawn(move || { handle_request_thread_function(new_queue); }));
	}
	
	match did_listen {
		Ok(listener) => server_loop(listener,queue),
		Err(e) => panic!("Unable to bind server to port 7878 : error {e:?}"),
	}

	for h in handles {
		match h.join() {
			Ok(_)=>(),
			Err(e)=>println!("{e:?}")
		}
	}
}
