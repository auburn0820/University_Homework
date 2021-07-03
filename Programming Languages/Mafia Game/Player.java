import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Scanner;

class Player {
    Socket playerSocket;
    boolean vote;
    boolean keepRunning;
    PlayerSendThread playerSendThread;
    BufferedReader bufferedReader;

    public Player() {
        initializeClient();
    }

    private void initializeClient() {
        try {
            keepRunning = true;
            vote = false;
            playerSocket = new Socket(InetAddress.getLocalHost().getCanonicalHostName(), MafiaServer.portNumber);
            InputStream inputStream = playerSocket.getInputStream();
            bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
            System.out.println("방에 입장하였습니다.");
            playerSendThread = new PlayerSendThread(playerSocket);
            playerSendThread.start();
        } catch (IOException e) {
            e.printStackTrace();
        }

        while(true) {
            if(!readMessage()) {
                exitGame();
                return;
            }
        }
    }

    private void exitGame() {
        if(!playerSocket.isClosed()) {
            try {
                playerSocket.close();
                if(playerSendThread.isAlive()) {
                    playerSendThread.interrupt();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        System.exit(1);
    }

    private boolean readMessage() {
        try {
            String message = bufferedReader.readLine();
            if("DisConnect".equals(message)) {
                playerSendThread.interrupt();
                exitGame();
                return false;
            } else if("투표를 진행하세요.".equals(message)) {
                System.out.println(message);
                synchronized (this) {
                    vote = true;
                }
            } else if (message != null) {
                System.out.println(message);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return true;
    }

    class PlayerSendThread extends Thread {
        Scanner scv;
        Socket playerSocket;
        PrintWriter printWriter;

        public PlayerSendThread(Socket playerSocket) {
            this.playerSocket = playerSocket;
            try {
                OutputStream outputStream = playerSocket.getOutputStream();
                printWriter = new PrintWriter(outputStream, true);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            while (!this.isInterrupted()) {
                if(!keepRunning)
                    break;
                if (!sendMessage())
                    break;
            }
        }

        private boolean sendMessage() {
            if(!keepRunning) {
                try {
                    playerSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                return false;
            }
            scv = new Scanner(System.in);
            String inputMessage = scv.nextLine();

            if(vote) {
                printWriter.println(inputMessage);
                synchronized (this) {
                    vote = false;
                }
                return true;
            }

            if ("quit".equalsIgnoreCase(inputMessage)) {
                printWriter.println(inputMessage);
                synchronized (this) {
                    keepRunning = false;
                }
                return false;
            } else if (inputMessage != null) {
                printWriter.println(inputMessage);
            }
            return true;
        }
    }

    public static void main(String[] args) {
        System.out.println("플레이어 코드 실행 중...");
        new Player();
    }
}
