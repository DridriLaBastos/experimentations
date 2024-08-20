use std::net::TcpListener;

fn server_loop(listener: TcpListener)
{
	for stream in listener.incoming()
	{
		match stream
		{
			Ok(s) => println!("Connection established {s:?}"),
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
