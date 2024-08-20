use std::{io::{BufRead, BufReader}, net::{TcpListener, TcpStream}};

fn handle_connection(stream: TcpStream)
{
	let buf_reader = BufReader::new(stream);
	let http_request: Vec<_> = buf_reader.lines().map(|result| result.unwrap()).take_while(|line| !line.is_empty()).collect();

	println!("Got request: {http_request:#?}");
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
	let did_listen = TcpListener::bind("127.0.0.1:80");

	match did_listen {
		Ok(listener) => server_loop(listener),
		Err(e) => panic!("Unable to bind server to port 7878 : error {e:?}"),
	}
}
