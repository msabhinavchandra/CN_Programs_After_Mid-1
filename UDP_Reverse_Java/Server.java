import java.net.*;

public class Server {
    public static void main(String[] args) {
        int port = 9876;
        try (DatagramSocket serverSocket = new DatagramSocket(port)) {
            System.out.println("Server is running on port " + port);
            byte[] receiveBuffer = new byte[1024];
            byte[] sendBuffer;
            while (true) {
                // Receive packet
                DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
                serverSocket.receive(receivePacket);
                String receivedSentence = new String(receivePacket.getData(), 0, receivePacket.getLength());
                System.out.println("Received: " + receivedSentence);
                // Reverse the sentence
                String reversedSentence = new StringBuilder(receivedSentence).reverse().toString();
                // Send the reversed sentence back
                sendBuffer = reversedSentence.getBytes();
                InetAddress clientAddress = receivePacket.getAddress();
                int clientPort = receivePacket.getPort();
                DatagramPacket sendPacket = new DatagramPacket(sendBuffer, sendBuffer.length,
                        clientAddress, clientPort);
                serverSocket.send(sendPacket);
                System.out.println("Sent: " + reversedSentence);
            }
        } catch (Exception e) {
            System.out.println("Server error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}