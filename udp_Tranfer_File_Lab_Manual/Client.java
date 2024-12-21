package udp_Tranfer_File_Lab_Manual;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Client {
    public static void main(String[] args) {
        String serverAddress = "127.0.0.1";
        int serverPort = 9876;
        String filePath = "Sender.txt";
        try (DatagramSocket clientSocket = new DatagramSocket();
                FileInputStream fileInputStream = new FileInputStream(new File(filePath))) {
            InetAddress serverInetAddress = InetAddress.getByName(serverAddress);
            byte[] sendBuffer = new byte[1024];
            int bytesRead;
            System.out.println("Sending file...");
            while ((bytesRead = fileInputStream.read(sendBuffer)) != -1) {
                DatagramPacket sendPacket = new DatagramPacket(sendBuffer, bytesRead, serverInetAddress,
                        serverPort);
                clientSocket.send(sendPacket);
            }
            // Send EOF signal
            String endSignal = "EOF";
            sendBuffer = endSignal.getBytes();
            DatagramPacket endPacket = new DatagramPacket(sendBuffer, sendBuffer.length,
                    serverInetAddress, serverPort);
            clientSocket.send(endPacket);
            System.out.println("File sent successfully.");
        } catch (IOException e) {
            System.out.println("Client error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}