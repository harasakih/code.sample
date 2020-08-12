import patlite
import time

def main():
    p = patlite.Patlite.get_instance()
	# ip-address, port
    p.set_dest()

    p.set_status("BLINK_G", p.ON)
    p.set_status("BUZZ_L", p.ON)
    p.commit()
   
    time.sleep(2)

    p.reset_status()
    p.commit()


if __name__ == '__main__':
    main()
