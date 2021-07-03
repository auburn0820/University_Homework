import java.io.*;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;

public class MafiaServer {
    // 포트 번호 메인 함수에 넣기.
    static int portNumber = 10000;
    private boolean vote;
    private int numberOfVote;
    private ArrayList<MafiaServerThread> joinedPlayer;
    private ServerSocket mafiaServerSocket;
    private Socket playerSocket;
    private Timer timer;
    private int playerNumber;
    private int mafiaNumber;
    private int terranNumber;
    private int maxPlayerNumber;
    private int voteTime;
    private boolean isGameEnded;
    private boolean checkGetName;
    private boolean synchronizedTurn;
    public ArrayList<String> playerRoleArray;

    public MafiaServer() {
        try {
            initializeVariable();
            fillPlayerRoleArray();
            String serverAddress = InetAddress.getLocalHost().getHostAddress();
            joinedPlayer = new ArrayList<MafiaServerThread>();
            mafiaServerSocket = new ServerSocket();
            mafiaServerSocket.bind(new InetSocketAddress(serverAddress, portNumber));
            listenPlayerSocket(mafiaServerSocket);
            processGameTurn();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void processGameTurn() {
        while(true) {
            System.out.print("");
            if(isGameEnded)
                System.exit(1);
            if(synchronizedTurn) {
                synchronizedTurn = false;
                turnTimer();
            }
        }
    }

    private void initializeVariable() {
        synchronizedTurn = true;
        numberOfVote = 0;
        vote = false;
        playerNumber = 0;
        isGameEnded = false;

        Scanner sc = new Scanner(System.in);

        System.out.print("최대 플레이어 수 : ");
        maxPlayerNumber = sc.nextInt();
        System.out.print("턴 당 시간(초) : ");
        voteTime = sc.nextInt();
        voteTime *= 1000;
        System.out.print("스크럴 수 : ");
        mafiaNumber = sc.nextInt();
        terranNumber = maxPlayerNumber - mafiaNumber;
    }

    private void exitGame() {
        System.out.println("Mafia Number : " + mafiaNumber);
        System.out.println("Terran Number : " + terranNumber);
        System.out.println("Player Number : " + playerNumber);

        if(mafiaNumber == 0) {
            sendMessageToAllPlayer("지구인의 승리입니다!");
        } else if(mafiaNumber == terranNumber) {
            sendMessageToAllPlayer("스크럴의 승리입니다!");
        }

        if(!joinedPlayer.isEmpty()) {
            sendMessageToAllPlayer("DisConnect");
            letOutAllPlayer();
            joinedPlayer.clear();
        }

        System.out.println("게임 종료");
        System.exit(1);
    }

    private void fillPlayerRoleArray() {
        playerRoleArray = new ArrayList<String>(maxPlayerNumber);
        for (int i = 0; i < maxPlayerNumber; i++) {
            if (i < terranNumber)
                playerRoleArray.add("지구인");
            else
                playerRoleArray.add("스크럴");
        }
    }

    private void addPlayerToList(MafiaServerThread mafiaServerThread) {
        joinedPlayer.add(mafiaServerThread);
    }

    private void listenPlayerSocket(ServerSocket mafiaServerSocket) {
        while (true) {
            checkGetName = false;
            try {
                if (checkNumberOfPlayer())
                    break;
                playerSocket = mafiaServerSocket.accept();
                MafiaServerThread mafiaServerThread = new MafiaServerThread(playerSocket);
                addPlayerToList(mafiaServerThread);
                mafiaServerThread.start();
                while (!checkGetName) { }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private boolean checkNumberOfPlayer() {
        if (playerNumber == maxPlayerNumber) {
            System.out.println("플레이어 모두 입장!");
            System.out.println("게임을 시작합니다.");
            System.out.println("한 턴에 60초 입니다. 턴이 끝나면 투표가 진행됩니다.");
            return true;
        } else
            return false;
    }

    private void sendMessageToAllPlayer(String message) {
        synchronized (joinedPlayer) {
            for (int i = 0; i < joinedPlayer.size(); i++) {
                MafiaServerThread mafiaServerThread = joinedPlayer.get(i);
                mafiaServerThread.sendMessage(message);
            }
        }
    }

    private boolean processVoteResult() {
        if(joinedPlayer.isEmpty())
            exitGame();

        MafiaServerThread mafiaServerThread = joinedPlayer
                .stream()
                .max(Comparator.comparing(player -> player.votedNumber))
                .get();


        for(int i = 0; i < joinedPlayer.size(); i++) {
            if(joinedPlayer.get(i) == mafiaServerThread)
                continue;
            if(joinedPlayer.get(i).votedNumber == mafiaServerThread.votedNumber) {
                sendMessageToAllPlayer("투표수가 같은 플레이어가 있어 투표는 무효 처리됩니다.");
                sendMessageToAllPlayer("날이 밝았습니다. 현재 인원은 " + playerNumber + "명 입니다.");
                return true;
            }
        }
        mafiaServerThread.keepRunning = false;
        sendMessageToAllPlayer(mafiaServerThread.playerName + "가 처형되었습니다.");
        sendMessageToAllPlayer(mafiaServerThread.playerName + "은(는) " + mafiaServerThread.playerRole + "이었습니다.");
        sendMessageToAllPlayer("날이 밝았습니다. 현재 인원은 " + (playerNumber - 1) + "명 입니다.");

        switch (mafiaServerThread.playerRole) {
            case "스크럴" :
                mafiaNumber--;
                break;
            case "지구인" :
                terranNumber--;
                break;
            default:
                break;
        }

        if(checkGameEnded()) {
            letOutAllPlayer();
            return false;
        }

        return true;
    }

    private boolean checkGameEnded() {
        if(mafiaNumber == 0) {
            letOutAllPlayer();
            return true;
        }
        if(terranNumber == mafiaNumber) {
            letOutAllPlayer();
            return true;
        }
        return false;
    }

    private void letOutAllPlayer() {
        for (MafiaServerThread mafiaServerThread : joinedPlayer) {
            mafiaServerThread.keepRunning = false;
        }
    }

    private void initializeVotedNumber() {
        for(MafiaServerThread mafiaServerThread : joinedPlayer) {
            mafiaServerThread.votedNumber = 0;
        }
    }

    private void vote() {
        if(playerNumber == 0)
            exitGame();

        initializeVotedNumber();
        System.out.println("투표 진행");
        sendMessageToAllPlayer("날이 저물었습니다. 투표를 진행하세요. (넘어갈 시, skip을 입력.)");
        synchronized (this) {
            vote = true;
        }
        while (vote) {
            System.out.print("");
            // 투표를 할 때까지 기다린다.
            if (numberOfVote == playerNumber) {
                System.out.println("투표가 완료되었습니다.");
                synchronized (this) {
                    numberOfVote = 0;
                    vote = false;
                    if(!(synchronizedTurn = processVoteResult())) {
                        sendMessageToAllPlayer("게임이 종료됩니다.");
                        exitGame();
                    }
                }
                break;
            }
        }
    }

    private void turnTimer() {
        timer = new Timer();
        TimerTask timerTask = new TimerTask() {
            @Override
            public void run() {
                vote();
            }
        };
        timer.schedule(timerTask, voteTime);
    }

    private void letOutPlayer(String message) {
        try {
            MafiaServerThread mafiaServerThread = findPlayer(message);
            mafiaServerThread.sendMessage("DisConnect");
            mafiaServerThread.playerSocket.close();
            synchronized (this) {
                for (int i = 0; i < joinedPlayer.size(); i++) {
                    if (joinedPlayer.get(i) == mafiaServerThread) {
                        joinedPlayer.remove(i);
                        break;
                    }
                }
                playerNumber--;
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private MafiaServerThread findPlayer(String message) {
        MafiaServerThread mafiaServerThread = joinedPlayer.stream()
                .filter(player -> player.playerName.equals(message))
                .findAny()
                .orElse(null);

        return mafiaServerThread;
    }

    class MafiaServerThread extends Thread {
        PrintWriter printWriter;
        BufferedReader bufferedReader;
        String playerRole;
        String playerName;
        Socket playerSocket;
        int votedNumber;
        boolean keepRunning;

        // 생성
        public MafiaServerThread(Socket playerSocket) {
            try {
                keepRunning = true;
                this.playerSocket = playerSocket;
                votedNumber = 0;
                InputStream inputStream = playerSocket.getInputStream();
                bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                OutputStream outputStream = playerSocket.getOutputStream();
                printWriter = new PrintWriter(outputStream, true);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            getPlayerName();
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    if (!communicationWithPlayers())
                        break;
                    Thread.sleep(10);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                System.out.println(this.playerName + " 종료");
            }
        }

        private void shuffleArray() {
            Random random = new Random();
            for (int i = 0; i < playerRoleArray.size(); i++) {
                int randomIndex = random.nextInt(playerRoleArray.size());
                String temp = playerRoleArray.get(randomIndex);
                playerRoleArray.set(randomIndex, playerRoleArray.get(i));
                playerRoleArray.set(i, temp);
            }
        }

        private void assignPlayerRole() {
            shuffleArray();
            for (int i = 0; i < joinedPlayer.size(); i++) {
                if (playerRoleArray.get(i) != null) {
                    playerRole = playerRoleArray.get(i);
                    playerRoleArray.remove(i);
                    break;
                }
            }
        }

        private void sendMessage(String message) {
            printWriter.println(message);
        }

        private void getPlayerName() {
            try {
                sendMessage("이름을 입력하세요.");
                this.playerName = bufferedReader.readLine();
                assignPlayerRole();
                sendMessageToAllPlayer(playerName + "님이 입장하셨습니다.");
                sendMessage(playerName + "님은 " + playerRole + "입니다.");
                increasePlayerNumber();
                checkGetName = true;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        private synchronized void increasePlayerNumber() {
            playerNumber++;
        }

        private boolean communicationWithPlayers() {
            try {
                if(!keepRunning) {
                    letOutPlayer(this.playerName);
                    return false;
                }

                String message = bufferedReader.readLine();

                if (vote) {
                    synchronized (this) {
                        numberOfVote++;
                    }
                    voteToPlayer(message);
                    return true;
                }
                if ("quit".equalsIgnoreCase(message)) {
                    sendMessageToAllPlayer(playerName + "님이 퇴장했습니다.");
                    keepRunning = false;
                    playerNumber--;
                    return false;
                } else if (message != null && !message.equals("")) {
                    sendMessageToAllPlayer(playerName + " : " + message);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            return true;
        }

        private MafiaServerThread findPlayer(String message) {
            MafiaServerThread mafiaServerThread = joinedPlayer.stream()
                    .filter(player -> player.playerName.equals(message))
                    .findAny()
                    .orElse(null);

            return mafiaServerThread;
        }

        private void voteToPlayer(String message) {
            if(message.equals("skip"))
                return;
            MafiaServerThread mafiaServerThread = findPlayer(message);
            if(mafiaServerThread != null)
                mafiaServerThread.votedNumber++;
        }
    }

    public static void main(String[] args) {
        new MafiaServer();
    }
}
