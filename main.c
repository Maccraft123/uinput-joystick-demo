#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>

int main(void)
{ 
	int fd = open("/dev/uinput", O_WRONLY);

	if (fd < 0)
	{
		perror("open /dev/uinput");
		return 1;
	}

	ioctl(fd, UI_SET_EVBIT, EV_KEY); // enable button/key handling

	ioctl(fd, UI_SET_KEYBIT, BTN_A);
	ioctl(fd, UI_SET_KEYBIT, BTN_B);
	ioctl(fd, UI_SET_KEYBIT, BTN_X);
	ioctl(fd, UI_SET_KEYBIT, BTN_Y);
	ioctl(fd, UI_SET_KEYBIT, BTN_TL);
	ioctl(fd, UI_SET_KEYBIT, BTN_TR);
	ioctl(fd, UI_SET_KEYBIT, BTN_START);
	ioctl(fd, UI_SET_KEYBIT, BTN_SELECT);
	ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_UP);
	ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_DOWN);
	ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_LEFT);
	ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_RIGHT);

	struct uinput_setup setup =
	{
		.name = "SSH Gamepad",
		.id =
		{
			.bustype = BUS_USB,
			.vendor  = 0x3,
			.product = 0x3,
			.version = 2,
		}
	};

	if (ioctl(fd, UI_DEV_SETUP, &setup))
	{
		perror("UI_DEV_SETUP");
		return 1;
	}

	if (ioctl(fd, UI_DEV_CREATE))
	{
		perror("UI_DEV_CREATE");
		return 1;
	}


	unsigned input = 0;


	while(1)
	{

		scanf("%u", &input);
		if ((input & 4096) > 0)
		{
			close(fd);
			return 0;
		}

		struct input_event ev[13];
		memset(&ev, 0, sizeof ev);


		ev[0].type = EV_KEY;
		ev[0].code = BTN_B;
		ev[0].value = input & 1;

		ev[1].type = EV_KEY;
		ev[1].code = BTN_Y;
		ev[1].value = input & 2;

		ev[2].type = EV_KEY;
		ev[2].code = BTN_SELECT;
		ev[2].value = input & 4;

		ev[3].type = EV_KEY;
		ev[3].code = BTN_START;
		ev[3].value = input & 8;

		ev[4].type = EV_KEY;
		ev[4].code = BTN_DPAD_UP;
		ev[4].value = input & 16;

		ev[5].type = EV_KEY;
		ev[5].code = BTN_DPAD_DOWN;
		ev[5].value = input & 32;

		ev[6].type = EV_KEY;
		ev[6].code = BTN_DPAD_LEFT;
		ev[6].value = input & 64;

		ev[7].type = EV_KEY;
		ev[7].code = BTN_DPAD_RIGHT;
		ev[7].value = input & 128;

		ev[8].type = EV_KEY;
		ev[8].code = BTN_A;
		ev[8].value = input & 256;

		ev[9].type = EV_KEY;
		ev[9].code = BTN_X;
		ev[9].value = input & 512;

		ev[10].type = EV_KEY;
		ev[10].code = BTN_TL;
		ev[10].value = input & 1024;

		ev[11].type = EV_KEY;
		ev[11].code = BTN_TR;
		ev[11].value = input & 2048;

		ev[12].type = EV_SYN;
		ev[12].code = SYN_REPORT;
		ev[12].value = 0;

		if(write(fd, &ev, sizeof ev) < 0)
		{
			perror("write");
			return 1;
		}
	}

	if(ioctl(fd, UI_DEV_DESTROY))
	{
		printf("UI_DEV_DESTROY");
		return 1;
	}

	close(fd);
	return 0;
}
