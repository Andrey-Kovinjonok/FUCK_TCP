import java.io.*;
import java.net.*;

public class Client {

	public static void main(String[] args) {
		try {
			newClient();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private static void newClient() throws IOException {
		String host = "127.0.0.1";
		int port = 7001;

		Socket socket = new Socket(host, port);
		DataInputStream dis = new DataInputStream(socket.getInputStream());
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		byte[] buf = new byte[64];

		boolean eof = false;
		while (true) {
			int len = readHeader(dis);
			if (len < 0) {
				break;
			}

			while (bos.size() < len) {
				int ret = dis.read(buf, 0, Math.min(len - bos.size(), buf.length));
				if (ret < 0) {
					// wtf?
					eof = true;
					break;
				}
				bos.write(buf, 0, ret);
			}

			if (eof) {
				break;
			}

			System.out.println(new String(bos.toByteArray()));
			bos.reset();
		}

		bos.close();
		dis.close();
		socket.close();
	}

	private static int readHeader(DataInputStream dis) throws IOException {
		try {
			char c = dis.readChar();
			System.out.print("header ");
			System.out.println((c >> 8)  - '0');
			return (c >> 8)  - '0';
		} catch (EOFException e) {
			return -1;
		}
	}
}
