import java.net.*;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        String serverHostname = "127.0.0.1";
        int serverPort = 9876;
        try (DatagramSocket clientSocket = new DatagramSocket();
                Scanner scanner = new Scanner(System.in)) {
            InetAddress serverAddress = InetAddress.getByName(serverHostname);
            byte[] sendBuffer;
            byte[] receiveBuffer = new byte[1024];
            System.out.println("Enter sentences to reverse (type 'exit' to quit):");
            while (true) {
                System.out.print(">> ");
                String inputSentence = scanner.nextLine();
                if ("exit".equalsIgnoreCase(inputSentence)) {
                    System.out.println("Exiting...");
                    break;
                }
                // Send input sentence to server
                sendBuffer = inputSentence.getBytes();
                DatagramPacket sendPacket = new DatagramPacket(sendBuffer, sendBuffer.length,
                        serverAddress, serverPort);
                clientSocket.send(sendPacket);
                // Receive reversed sentence from server
                DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
                clientSocket.receive(receivePacket);
                String reversedSentence = new String(receivePacket.getData(), 0, receivePacket.getLength());
                System.out.println("Server response: " + reversedSentence);
            }
        } catch (Exception e) {
            System.out.println("Client error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}