#include "os.h"
#include "panic.h"
#include "stdio.h"
#include "Drivers/ac97.h"
#include "Net/net.h"
#include "Net/tcpsocket.h"
#include "Net/udpsocket.h"
#include "Net/dns.h"
#include "Net/dhcp.h"
#include "Net/http.h"
#include "Process/scheduler.h"
#include "Drawing/font.h"
#include "GUI/GUI.h"
#include "System.h"

#include "Test/Mandelbrot.h"
#include "Test/TextEdit.h"

#include "debug.h"

#include "../3DGame/3DGame.h"


void GUI()
{
	gui::WindowManager::Init();

	Scheduler::InsertThread(Scheduler::CreateKernelThread(apps::RunTextEdit));
	Scheduler::InsertThread(Scheduler::CreateKernelThread(apps::RunTextEdit));

	//Task::InsertThread(Task::CreateThread(gui::WindowManager::Start));
	gui::WindowManager::Start();
}

GC gameGC;

void render()
{
	while (1)
	{
		Drawing::BitBlt(gameGC, 0, 0, gameGC.width, gameGC.height, Drawing::gc_direct, 0, 0);
	}
}

void Game()
{
	gameGC = Drawing::gc_direct;
	//gameGC = GC(Drawing::gc_direct, 100, 100, 800, 600);

	//gameGC = GC::CreateGraphics(500, 500);
	//Task::InsertThread(Task::CreateThread(render));

	while (1)
	{
		GEngine* engine = new GEngine(gameGC);
		MyGame* game = new MyGame();
		engine->StartGame(game);
	}
}

void Audio()
{
	PCI_DEVICE* dev = PCI::GetDevice(4, 1, 0);

	if (dev)
	{
		Debug::Print("Found audio device\n");
		AC97::Init(dev);
	}
	else
	{
		Debug::Print("Audio device not found!\n");
	}

	while (1);
}

void _Font()
{
	char* name = "arial";
	//name = "calibri";
	name = "consolas";

	Font font(name);
	while (1);
}

#include "API/api.h"
void Syscall()
{
	api_test();
}

#include "Process/process.h"
void _Process()
{
	Process::Create("Test.exe");
}

void T2()
{
	const char* text = "1";

	while (1)
	{
		_asm
		{
			mov eax, 2
			mov ebx, text
			int 0x80
		}

		_asm pause
	}
}

void T1()
{
	uint32 colors[] = {
		COLOR_RED,
		COLOR_BLUE,
		COLOR_GREEN,
		COLOR_CYAN,
		COLOR_MAGENTA,
		COLOR_YELLOW
	};

	int t = 0;

	while (1)
	{
		Debug::color = colors[(t++ / 10) % 6];
		Debug::bcolor = 0;
		_asm pause
	}
}

void OS::Main()
{
	uint32 stack = (uint32)PMem::AllocBlocks(1);
	uint32 virtStack = (uint32)0x50000000;
	VMem::MapPhysAddr(VMem::GetCurrentDir(), (uint32)stack, (uint32)virtStack, PTE_PRESENT | PTE_WRITABLE | PTE_USER);
	VMem::MapPhysAddr(VMem::GetCurrentDir(), (uint32)T2, (uint32)T2, PTE_PRESENT | PTE_WRITABLE | PTE_USER);

	Thread* t1 = Scheduler::CreateKernelThread(T1);
	//Thread* t2 = Scheduler::CreateKernelThread(T2);
	Thread* t2 = Scheduler::CreateUserThread(T2, (void*)(virtStack + BLOCK_SIZE));

	Scheduler::InsertThread(t1);
	Scheduler::InsertThread(t2);

	while (1)
		_asm pause;

	//Net::Init();

	//Mandelbrot mandelbrot(Drawing::gc_direct);
	//mandelbrot.Run();

	FS::Init();

	//GUI();
	Game();
	//Audio();
	//_Font();
	//Syscall();
	_Process();
	while (1);

	PIT::Sleep(2000);
	GUI();

	IPv4Address addr = Net::GatewayIPv4;
	addr.n[3] = 1;

	//51.175.48.225
	addr.n[0] = 0x33;
	addr.n[1] = 0xAF;
	addr.n[2] = 0x30;
	addr.n[3] = 0xE1;

	//192.168.0.123
	addr.n[0] = 0xC0;
	addr.n[1] = 0xA8;
	addr.n[2] = 0x00;
	addr.n[3] = 0x7B;

	//Debug::Print("Mem: %ui\n", Memory::num_free);
	//char* c = new char[123];
	//Debug::Print("Mem: %ui\n", Memory::num_free);
	//c = new char[123];
	//Debug::Print("Mem: %ui\n", Memory::num_free);
	//c = new char[0xFFF];
	//Debug::Print("Mem: %ui\n", Memory::num_free);
	//while (1);
	//google.com
	//addr.n[0] = 0xD8;
	//addr.n[1] = 0x3A;
	//addr.n[2] = 0xD1;
	//addr.n[3] = 0x8E;

	UdpSocket* us = UDP::CreateSocket(1881);

	Debug::Print("Ready\n");


	while (1)
	{
		IPv4Address recA;
		uint8* recB;
		int length = us->Receive(recB, recA);
		recB[length] = 0;

		Debug::Print("%s\n", recB);

		/*while (Keyboard::GetLastKey().key == KEY_INVALID || !Keyboard::GetLastKey().pressed);
		KEY_PACKET key = Keyboard::GetLastKey();
		Keyboard::DiscardLastKey();

		char* data = "hello ..";
		us->Send(addr, (uint8*)data, strlen(data));

		Debug::Print("Sent\n");*/
	}


	int port = 80;
	char* http = "GET /test.php HTTP/1.1\r\nHost: 192.168.0.123\r\n\r\n";

	while (1)
	{
		//DNS::Query(Net::intf, "andyhk.ga");
		//Debug::Print("Sent DNS\n");

		//DHCP::DoStuff(Net::intf, Net::BroadcastIPv4);
		WebClient client;
		client.Send("andyhk.ga", (uint8*)http, strlen(http));

		while (Keyboard::GetLastKey().key == KEY_INVALID || !Keyboard::GetLastKey().pressed);
		KEY_PACKET key = Keyboard::GetLastKey();
		Keyboard::DiscardLastKey();

		Net::PrintIP("andyhk.ga: ", DNS::LookupAddress("andyhk.ga"));

		//Debug::Print("Mem: %ui\n", Memory::num_free);
		//while (1);
	}

	TcpSocket* ts = new TcpSocket;

	Debug::Print("Connecting...\n");
	bool b = ts->Connect(addr, port);

	while (1)
	{
		while (Keyboard::GetLastKey().key == KEY_INVALID || !Keyboard::GetLastKey().pressed);
		KEY_PACKET key = Keyboard::GetLastKey();
		Keyboard::DiscardLastKey();

		if (key.key == KEY_SPACE)
		{
			Debug::Print("Connecting...\n");
			bool b = ts->Connect(addr, port);
		}

		if (key.key == KEY_S)
		{
			char buf[256];
			vprintf(buf, "OS ticks: %i", PIT::ticks);
			int length = strlen(buf);

			Debug::Print("Sending...\n");
			ts->Send((uint8*)http, strlen(http));
		}

		if (key.key == KEY_ESCAPE)
		{
			bool b = ts->Close();
		}
	}
}