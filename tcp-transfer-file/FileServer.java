import java.io.*;
import java.net.*;

public class FileServer {
    public static void main(String[] args) {
        int port = 65432;
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Server is listening on port " + port);
            while (true) {
                Socket socket = serverSocket.accept();
                System.out.println("Client connected.");
                try (
                        InputStream inputStream = socket.getInputStream();
                        DataInputStream dataInputStream = new DataInputStream(inputStream);
                        FileOutputStream fileOutputStream = new FileOutputStream("testfile2.txt")) {
                    System.out.println("Receiving file...");
                    // Read file data from the client
                    long fileSize = dataInputStream.readLong();
                    byte[] buffer = new byte[4096];
                    int bytesRead;
                    long totalBytesRead = 0;
                    while (totalBytesRead < fileSize && (bytesRead = dataInputStream.read(buffer)) != -1) {
                        fileOutputStream.write(buffer, 0, bytesRead);
                        totalBytesRead += bytesRead;
                    }
                    System.out.println("File received successfully.");
                } catch (IOException e) {
                    System.out.println("Error handling file transfer: " + e.getMessage());
                }
                socket.close();
                System.out.println("Connection closed.");
            }
        } catch (IOException e) {
            System.out.println("Server exception: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
