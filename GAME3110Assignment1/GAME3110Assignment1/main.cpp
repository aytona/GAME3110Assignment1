#include "MessageIdentifiers.h"
#include "RakPeer.h"
#include "RakPeerInterface.h"
#include "ReadyEvent.h"
#include "FullyConnectedMesh2.h"
#include "ConnectionGraph2.h"
#include "Gets.h"
#include <thread>
#include <vector>
#include <mutex>

#include "player.h"
#include "angel.h"
#include "devil.h"
#include "fallen.h"

using namespace RakNet;

enum {
    ID_GB3_CREATE_PLAYER = ID_USER_PACKET_ENUM,
    ID_GB3_HEAL,
    ID_GB3_ATTACK,
    ID_GB3_POWERUP,
    ID_GB3_SPECIAL,
};

enum ReadyEventIDs {
    ID_RE_PLAYER_JOIN = 0,
    ID_RE_PLAYER_TURN = 1,
    ID_RE_PLAYER_NO_TURN = 2,
    ID_RE_GAME_OVER,
};

RakPeerInterface* g_rakPeerInterface = nullptr;

ReadyEvent g_readyEventPlugin;

FullyConnectedMesh2 g_fcm2;
ConnectionGraph2 g_cg2;
NetworkIDManager g_networkIDManager;

int g_startingPort = 6500;
bool g_isGameRunning = false;
bool g_isRunning = true;

std::mutex g_playerMutex;
std::vector<Player*> m_players;

void Update();
void PacketListener();
void InputListener();
void DisplayHelp();

int main() {
    g_rakPeerInterface = RakPeerInterface::GetInstance();
    g_rakPeerInterface->AttachPlugin(&g_readyEventPlugin);
    g_rakPeerInterface->AttachPlugin(&g_fcm2);
    g_rakPeerInterface->AttachPlugin(&g_cg2);

    g_fcm2.SetAutoparticipateConnections(true);
    g_fcm2.SetConnectOnNewRemoteConnection(true, "");
    g_cg2.SetAutoProcessNewConnections(true);

    const unsigned int maxConnections = 3;

    while (IRNS2_Berkley::IsPortInUse(g_startingPort, "127.0.0.1", AF_INET, SOCK_DGRAM) == true) {
        g_startingPort++;
    }

    SocketDescriptor sd(g_startingPort, "127.0.0.1");
    StartupResult result = g_rakPeerInterface->Startup(maxConnections, &sd, 1);
    g_rakPeerInterface->SetMaximumIncomingConnections(maxConnections);
    if (result != RAKNET_STARTED) {
        printf("RakNet won't start %i\n", result);
        system("pause");
        exit(0);
    }

    printf("Started Port: %i\n", g_startingPort);
    printf("Start listening for packets..\n");

    std::thread packetListernerThread(PacketListener);

    printf("Press y to connect\n");
    
    char userInput[32];
    Gets(userInput, sizeof(userInput));
    if (userInput[0] == 'y' || userInput[0] == 'Y') {
        printf("Please enter the port to connect to: ");
        char port[32];
        Gets(port, sizeof(port));
        int portNum = atoi(port);
        ConnectionAttemptResult caResult = g_rakPeerInterface->Connect("127.0.0.1", portNum, nullptr, 0);
        if (caResult != CONNECTION_ATTEMPT_STARTED) {
            printf("Can't connect %i\n", caResult);
            system("pause");
            exit(0);
        }
        printf("Connected.\n");
    }

    g_readyEventPlugin.AddToWaitList(ID_RE_PLAYER_JOIN, g_rakPeerInterface->GetMyGUID());

    printf("Choose your Character\n");
    printf("'p' = Player\n");
    printf("'a' = Angel\n");
    printf("'d' = Devil\n");
    printf("'f' = Fallen\n");
    Gets(userInput, sizeof(userInput));
    if (userInput[0] == 'p' || userInput[0] == 'P') {
        Player* player = new Player();
        player->SetIsMaster(true);
        player->SetNetworkIDManager(&g_networkIDManager);
        g_playerMutex.lock();
        m_players.insert(m_players.begin(), player);
        g_playerMutex.unlock();

        std::thread inputListener(InputListener);
        std::thread playerUpdate(Update);

        g_readyEventPlugin.SetEvent(ID_RE_PLAYER_JOIN, true);
        packetListernerThread.join();
    } else if (userInput[0] == 'p' || userInput[0] == 'P') {
        Player* player = new Angel();
        player->SetIsMaster(true);
        player->SetNetworkIDManager(&g_networkIDManager);
        g_playerMutex.lock();
        m_players.insert(m_players.begin(), player);
        g_playerMutex.unlock();

        std::thread inputListener(InputListener);
        std::thread playerUpdate(Update);

        g_readyEventPlugin.SetEvent(ID_RE_PLAYER_JOIN, true);
        packetListernerThread.join();
    } else if (userInput[0] == 'p' || userInput[0] == 'P') {
        Player* player = new Devil();
        player->SetIsMaster(true);
        player->SetNetworkIDManager(&g_networkIDManager);
        g_playerMutex.lock();
        m_players.insert(m_players.begin(), player);
        g_playerMutex.unlock();

        std::thread inputListener(InputListener);
        std::thread playerUpdate(Update);

        g_readyEventPlugin.SetEvent(ID_RE_PLAYER_JOIN, true);
        packetListernerThread.join();
    } else if (userInput[0] == 'p' || userInput[0] == 'P') {
        Player* player = new Fallen();
        player->SetIsMaster(true);
        player->SetNetworkIDManager(&g_networkIDManager);
        g_playerMutex.lock();
        m_players.insert(m_players.begin(), player);
        g_playerMutex.unlock();

        std::thread inputListener(InputListener);
        std::thread playerUpdate(Update);

        g_readyEventPlugin.SetEvent(ID_RE_PLAYER_JOIN, true);
        packetListernerThread.join();
    } else {
        g_isRunning = false;
        printf("WRONG\n");
    }

    return 0;
}

void Update() {
    while (g_isRunning) {
        while (g_isGameRunning) {
            g_playerMutex.lock();
            for each (Player* player in m_players) {
                player->Update();
            }
            g_playerMutex.unlock();
        }
        Sleep(100);
    }
}

void PacketListener() {
    printf("Listening for packets...\n");
    Packet* packet;
    while (g_isRunning) {
        for (packet = g_rakPeerInterface->Receive(); packet != nullptr; g_rakPeerInterface->DeallocatePacket(packet), packet = g_rakPeerInterface->Receive()) {
            unsigned short packetIdentifier = packet->data[0];
            switch (packetIdentifier) {
            case ID_CONNECTION_REQUEST_ACCEPTED:
            case ID_NEW_INCOMING_CONNECTION:
                printf("ID = %i\n", packetIdentifier);
                g_readyEventPlugin.AddToWaitList(ID_RE_PLAYER_JOIN, packet->guid);
                break;
            case ID_READY_EVENT_ALL_SET:
                printf("ID = %i\n", packetIdentifier);
                {
                    BitStream bs(packet->data, packet->length, false);
                    bs.IgnoreBits(sizeof(MessageID));
                    int readyEventId;
                    bs.Read(readyEventId);
                    if (readyEventId == ID_RE_PLAYER_JOIN && !g_isGameRunning) {
                        g_isGameRunning = true;
                        BitStream bs;
                        bs.Write((unsigned char)ID_GB3_CREATE_PLAYER);
                        bs.Write(m_players[0]->GetNetworkID());
                        g_rakPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
                    }
                }
                break;
            case ID_GB3_CREATE_PLAYER:
            {
                printf("Creating player..\n");
                BitStream bs(packet->data, packet->length, false);
                bs.IgnoreBytes(sizeof(MessageID));
                RakNet::NetworkID netID;
                bs.Read(netID);
                // Set player defaults
                Player* player = new Player();

                // Locks the racers vector so others can't add to it yet
                g_playerMutex.lock();
                m_players.push_back(player);
                g_playerMutex.unlock();
            }
            case ID_CONNECTION_ATTEMPT_FAILED:
            case ID_ALREADY_CONNECTED:
            case ID_NO_FREE_INCOMING_CONNECTIONS:
            case ID_READY_EVENT_SET:
            case ID_READY_EVENT_UNSET:
            default:
                printf("ID = %i\n", packetIdentifier);
                break;
            }
        }
    }
}

void InputListener() {
    DisplayHelp();
    static char* heal = "h";
    static char* attack = "a";
    static char* powerup = "p";
    static char* special = "s";
    static char* enemies = "e";
    static char* quit = "q";
    static char* help = "p";
    while (g_isRunning) {
        while (g_isGameRunning) {
            char input[32];
            Gets(input, sizeof(input));
            if (strcmp(input, heal) == 0) {
                m_players[0]->Heal();
                BitStream bs;
                bs.Write((unsigned char)ID_GB3_HEAL);
                bs.Write(m_players[0]->GetNetworkID());
                g_rakPeerInterface->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
            } else if (strcmp(input, attack) == 0) {
                m_players[0]->Attack(m_players);
            } else if (strcmp(input, powerup) == 0) {

            } else if (strcmp(input, special) == 0) {

            } else if (strcmp(input, quit) == 0) {

            } else if (strcmp(input, help) == 0) {

            }
        }
        Sleep(100);
    }
}

void DisplayHelp() {
    printf("*********Player Commands*********************\n");
    printf("'h' = Heal\n");
    printf("'a' = Attack\n");
    printf("'p' + # = Power Up (speed, power, heal)\n");
    printf("'s' = Special (One time move)\n");
    printf("'e' = Enemies\n");
    printf("'q' = Quit\n");
    printf("'p' = Help\n");
    printf("*********************************************\n");
}