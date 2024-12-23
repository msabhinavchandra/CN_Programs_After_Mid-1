import java.io.*;
import java.net.*;

public class FileClient {
    public static void main(String[] args) {
        String hostname = "127.0.0.1";
        int port = 65432;
        try (Socket socket = new Socket(hostname, port)) {
            System.out.println("Connected to the server.");
            // Specify the file to send
            File file = new File("testfile.txt");
            if (!file.exists()) {
                System.out.println("File does not exist.");
                return;
            }
            try (
                    FileInputStream fileInputStream = new FileInputStream(file);
                    OutputStream outputStream = socket.getOutputStream();
                    DataOutputStream dataOutputStream = new DataOutputStream(outputStream)) {
                System.out.println("Sending file: " + file.getName());
                // Send file size to the server
                dataOutputStream.writeLong(file.length());
                // Send file data
                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                    dataOutputStream.write(buffer, 0, bytesRead);
                }
                System.out.println("File sent successfully.");
            } catch (IOException e) {
                System.out.println("Error sending file: " + e.getMessage());
            }
        } catch (UnknownHostException e) {
            System.out.println("Server not found: " + e.getMessage());
        } catch (IOException e) {
            System.out.println("I/O error: " + e.getMessage());
        }
    }
}
