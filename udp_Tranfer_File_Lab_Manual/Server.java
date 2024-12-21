package udp_Tranfer_File_Lab_Manual;

import java.io.FileOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class Server {
    public static void main(String[] args) {
        int port = 9876;
        String outputFileName = "received_file.txt";
        try (DatagramSocket serverSocket = new DatagramSocket(port);
                FileOutputStream fileOutputStream = new FileOutputStream(outputFileName)) {
            System.out.println("Server is running on port " + port);
            byte[] receiveBuffer = new byte[1024];
            DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
            System.out.println("Waiting for file...");
            while (true) {
                serverSocket.receive(receivePacket);
                // Check if it's the end of the file
                String endSignal = new String(receivePacket.getData(), 0, receivePacket.getLength());
                if ("EOF".equals(endSignal)) {
                    System.out.println("File transfer complete.");
                    break;
                }
                // Write data to the file
                fileOutputStream.write(receivePacket.getData(), 0, receivePacket.getLength());
            }
        } catch (IOException e) {
            System.out.println("Server error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
