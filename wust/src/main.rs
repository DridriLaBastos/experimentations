use std::{borrow::BorrowMut, fs, io::{BufRead, BufReader, Read, Write}, net::{TcpListener, TcpStream}, ops::Deref, str::FromStr, thread::sleep, time::Duration, usize};

mod threading;

fn handle_request_thread_function(id: usize, queue: std::sync::Arc<threading::Queue<&str>>)
{
	for _ in 0..5 {
		// queue.as_ref().get();
		sleep(Duration::from_secs(5));
		println!("handle_request_thread_function {id}")
	}
}

//TODO: To be quick I use unwrap, but those cases should be handled properly
// fn handle_data_received(mut stream: TcpStream, queue: std::sync::Arc<wust::Queue<&str>>)
// {
// 	let buf_reader = BufReader::new(&stream);

// 	let http_request: Vec<_> = buf_reader
// 						.lines()
// 						.map(|result| result.unwrap())
// 						.take_while(|line| !line.is_empty())
// 						.collect();
	
	

// 	// let http_request: Vec<_> = buf_reader.lines().map(|result| result.unwrap()).take_while(|line| !line.is_empty()).collect();
// 	// let http_request_header = http_request[0].as_str();

// 	// let (html_response_status, filename) = match http_request_header {
// 	// 	"GET / HTTP/1.1" => ("HTTP/1.1 200 OK", std::path::Path::new("test/index.html")),
// 	// 	"GET /sleep HTTP/1.1" => {
// 	// 		std::thread::sleep(std::time::Duration::from_secs(5));
// 	// 		("HTTP/1.1 200 OK", std::path::Path::new("test/index.html"))
// 	// 	}
// 	// 	_ => ("HTTP/1.1 404 NOT FOUND", std::path::Path::new("test/404.html")),
// 	// };

// 	// println!("Request: {http_request:#?}");

// 	// let html_response_content = fs::read_to_string(filename).unwrap_or(
// 	// 	String::from_str("<!DOCTYPE html>
// 	// 				<html lang=\"en\">
// 	// 				<head>
// 	// 					<meta charset=\"UTF-8\">
// 	// 					<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
// 	// 					<title>Document</title>
// 	// 				</head>
// 	// 				<body>
// 	// 					<h1>Internal sever error</h1>
// 	// 				</body>
// 	// 				</html>").unwrap()
// 	// );

// 	// let html_response_content_length = html_response_content.len();

// 	// let response = format!("{html_response_status}\r\nContent-Length: {html_response_content_length}\r\n\r\n{html_response_content}");
// 	// stream.write_all(response.as_bytes()).unwrap();

// }

fn server_loop(listener: TcpListener, queue: std::sync::Arc<threading::Queue<&str>>)
{
	// for stream in listener.incoming()
	// {
	// 	match stream
	// 	{
	// 		Ok(s) => handle_data_received(s, queue),
	// 		Err(e) => panic!("Unable to create a connection : {e:?}"),
	// 	}
	// }

	for _ in 0..5 {
		sleep(Duration::from_secs(2));
		println!("server_loop")
	}
}

fn main() {
	let did_listen = TcpListener::bind("127.0.0.1:7878");
	let thread_number: usize = 2;
	let queue = std::sync::Arc::new(threading::Queue::new(5));
	let mut handles = std::vec::Vec::new();

	for id in 0..thread_number {
		let new_queue = std::sync::Arc::clone(&queue);
		handles.push(std::thread::spawn(move || { handle_request_thread_function(id,new_queue); }));
	}  

	match did_listen {
		Ok(listener) => server_loop(listener,queue),
		Err(e) => panic!("Unable to bind server to port 7878 : error {e:?}"),
	}

	for h in handles {
		match(h.join()) {
			Ok(_)=>(),
			Err(e)=>println!("{e:?}")
		}
	}
}
