use std::{fs::read_to_string, path::Path};

fn main() {
	let args: Vec<String> = std::env::args().collect();
	let maybe_path = args.get(1);

	match maybe_path {
		None => panic!("A path is required as argument"),
		Some(path_str) => try_compress(path_str.as_str()),
	}
}

fn try_compress(path: &str)
{
	let content = read_to_string(path).expect(format!("Unable to read file {path}").as_str());
	let content_byte = content.as_bytes();

	println!("'{content_byte:?}'");
}
