use std::{io::{BufRead, BufReader, Write}, net::{TcpListener, TcpStream}};

//TODO: To be quick I use unwrap, but those cases should be handled properly

fn handle_connection(mut stream: TcpStream)
{
	let buf_reader = BufReader::new(&stream);
	let http_request: Vec<_> = buf_reader.lines().map(|result| result.unwrap()).take_while(|line| !line.is_empty()).collect();

	println!("Request: {http_request:#?}");

	let html_response_status = "HTTP/1.1 200 OK";
	let html_response_content = "
		<!DOCTYPE html>
			<html lang=\"en\">
			<head>
				<meta charset=\"UTF-8\">
				<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
				<title>Document</title>
			</head>
			<body>
				<h1>Hello Wust World</h1>
				<p>This page have been loaded from my rust server</p>
			</body>
			</html>
		";
	let html_response_content_length = html_response_content.len();

	let response = format!("{html_response_status}\r\nContent-Length: {html_response_content_length}\r\n\r\n{html_response_content}");
	stream.write_all(response.as_bytes()).unwrap();
}

fn server_loop(listener: TcpListener)
{
	for stream in listener.incoming()
	{
		match stream
		{
			Ok(s) => handle_connection(s),
			Err(e) => panic!("Unable to create a connection : {e:?}"),
		}
	}
}

fn main() {
	let did_listen = TcpListener::bind("127.0.0.1:7878");

	match did_listen {
		Ok(listener) => server_loop(listener),
		Err(e) => panic!("Unable to bind server to port 7878 : error {e:?}"),
	}
}
