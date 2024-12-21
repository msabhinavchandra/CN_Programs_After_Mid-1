package DNS_Lab_Manual;

import java.net.*;
import java.util.Scanner;

public class SimpleDNSResolver {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Welcome to the Simple DNS Resolver!");
        System.out.println("Enter a domain name to resolve (or type 'exit' to quit):");
        while (true) {
            System.out.print("Domain: ");
            String domain = scanner.nextLine();
            if ("exit".equalsIgnoreCase(domain)) {
                System.out.println("Exiting DNS Resolver. Goodbye!");
                break;
            }
            try {
                InetAddress[] addresses = InetAddress.getAllByName(domain);
                System.out.println("IP addresses for " + domain + ":");
                for (InetAddress address : addresses) {
                    System.out.println(" - " + address.getHostAddress());
                }
            } catch (UnknownHostException e) {
                System.out.println("Could not resolve domain: " + e.getMessage());
            }
        }
        scanner.close();
    }
}