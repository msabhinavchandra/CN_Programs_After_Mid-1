#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define WINDOW_SIZE 4   // Size of the sliding window
#define TOTAL_FRAMES 10 // Total number of frames to be sent
#define TIMEOUT 3       // Timeout duration in seconds
int send_frame(int frame)
{
    // Simulate random loss of frames (e.g., frame 2 is lost)
    if (rand() % 5 == 0)
    {
        printf("[Sender] Frame %d lost during transmission.\n", frame);
        return 0; // Simulate frame loss
    }
    printf("[Sender] Frame %d sent successfully.\n", frame);
    return 1; // Frame sent successfully
}
int receive_ack(int frame)
{
    // Simulate random loss of ACKs (e.g., ACK for frame 3 is lost)
    if (rand() % 7 == 0)
    {
        printf("[Receiver] ACK for Frame %d lost.\n", frame);
        return 0; // Simulate ACK loss
    }
    printf("[Receiver] ACK for Frame %d received successfully.\n", frame);
    return 1; // ACK received successfully
}
void sliding_window_protocol()
{
    int base = 0;                 // Oldest unacknowledged frame
    int next_frame = 0;           // Next frame to be sent
    int acks[TOTAL_FRAMES] = {0}; // Array to track acknowledgments
    time_t timers[TOTAL_FRAMES];  // Timers for each frame
    while (base < TOTAL_FRAMES)
    {
        // Send frames within the window
        while (next_frame < base + WINDOW_SIZE && next_frame < TOTAL_FRAMES)
        {
            if (!acks[next_frame])
            {
                send_frame(next_frame);
                timers[next_frame] = time(NULL); // Start the timer
            }
            next_frame++;
        }
        // Check for ACKs or timeouts
        for (int i = base; i < next_frame; i++)
        {
            if (acks[i])
            {
                continue; // Skip acknowledged frames
            }
            // Simulate receiving an acknowledgment
            if (receive_ack(i))
            {
                acks[i] = 1; // Mark frame as acknowledged
                // Slide the window if the base frame is acknowledged
                while (base < TOTAL_FRAMES && acks[base])
                {
                    printf("[Sender] Sliding window. Base frame is now %d.\n", base + 1);
                        base++;
                }
            }
            else
            {
                // Check for timeout
                if (difftime(time(NULL), timers[i]) > TIMEOUT)
                {
                    printf("[Sender] Timeout for Frame %d. Resending all frames from %d.\n", i, base);
                    // Resend all frames from the base frame
                    next_frame = base;
                    break;
                }
            }
        }
    }
    printf("[Sender] All frames sent and acknowledged successfully.\n");
}
int main()
{
    srand(time(NULL)); // Seed for random number generation
    printf("[Sender] Starting Sliding Window Protocol with Go-Back-N.\n");
    sliding_window_protocol();
    printf("[Sender] Transmission completed.\n");
    return 0;
}
