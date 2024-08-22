use std::{fs, io::{BufRead, BufReader, Write}, net::{TcpListener, TcpStream}, str::FromStr};

//TODO: To be quick I use unwrap, but those cases should be handled properly

fn handle_connection(mut stream: TcpStream)
{
	let buf_reader = BufReader::new(&stream);
	let http_request: Vec<_> = buf_reader.lines().map(|result| result.unwrap()).take_while(|line| !line.is_empty()).collect();
	let http_request_ok = &http_request[0] == "GET / HTTP/1.1";

	let (html_response_status, filename) = if http_request_ok
			{ ("HTTP/1.1 200 OK", std::path::Path::new("test/index.html"))}
		else
			{ ("HTTP/1.1 404 NOT FOUND", std::path::Path::new("test/404.html"))};

	println!("Request: {http_request:#?}");

	let html_response_content = fs::read_to_string(filename).unwrap_or(
		String::from_str("<!DOCTYPE html>
					<html lang=\"en\">
					<head>
						<meta charset=\"UTF-8\">
						<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
						<title>Document</title>
					</head>
					<body>
						<h1>Internal sever error</h1>
					</body>
					</html>").unwrap()
	);

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
