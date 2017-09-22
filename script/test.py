"""
    ctc-single-process
"""

"""
    emb.func1();
    print('RBC')
    time.sleep(5)
    print("DateTime " + time.strftime("%c"))
    print('WBC1')
    time.sleep(5)
    print("DateTime " + time.strftime("%c"))
    print('WBC2')
    time.sleep(5)
    print("DateTime " + time.strftime("%c"))
    print('WBC3')
    time.sleep(5)
    print("DateTime " + time.strftime("%c"))
    print('DYE')
    time.sleep(5)
    print("DateTime " + time.strftime("%c"))

	{ "emb_set_cen_rpm", emb_set_cen_rpm, METH_VARARGS, "set_cen_rpm" },
	{ "emb_set_cen_rcf", emb_set_cen_rcf, METH_VARARGS, "set_cen_rcf" },
	{ "emb_set_cen_temp", emb_set_cen_temp, METH_VARARGS, "set_cen_temp" },
	{ "emb_set_cen_duration", emb_set_cen_duration, METH_VARARGS, "set_cen_duration" },
	{ "emb_set_cen_ctrl", emb_set_cen_ctrl, METH_VARARGS, "set_cen_ctrl" },
#endif
"""

import time
import emb

emb.set_mp1_pos(10, 0, 0, 0, 0, 0x01) # x
emb.set_mp1_pos(0, 20, 0, 0, 0, 0x02) # y
emb.set_mp1_pos(0, 0, 30, 0, 0, 0x04) # z1
emb.set_mp1_pos(0, 0, 0, 40, 0, 0x08) # z2
emb.set_mp1_pos(0, 0, 0, 0, 50, 0x10) # z3
emb.set_cen_rpm(2000)
emb.set_cen_rcf(500)
emb.set_cen_temp(30)
emb.set_cen_duration(60)
emb.set_cen_ctrl(1)
emb.set_lgw_pos(100) # mm
emb.set_lgw_rpm(200)
emb.set_lgw_ramp(1000)
emb.set_lgw_pulse(4194304)
emb.set_grip_on(1)
emb.set_grip_off(1)
emb.set_grip_on(1)
emb.set_grip_off(1)

print("hello")

def main():
    emb.set_mp1_pos(10, 0, 0, 0, 0, 0x01)
    emb.set_mp1_pos(0, 20, 0, 0, 0, 0x02)
    emb.set_mp1_pos(0, 0, 30, 0, 0, 0x04)
    emb.set_mp1_pos(0, 0, 0, 40, 0, 0x08)
    emb.set_mp1_pos(0, 0, 0, 0, 50, 0x10)
    emb.set_cen_rpm(2000)
    emb.set_cen_rcf(500)
    emb.set_cen_temp(30)
    emb.set_cen_duration(60)
    emb.set_cen_ctrl(1)
    print("hello")

if __name__ == '__main__':
#if __name__ == "ctc-single-process":
    main()

#print('__name__: ' + __name__)
#print('__main__: ' + __main__)

"""
print('RBC')
time.sleep(5)
print("DateTime " + time.strftime("%c"))
print('WBC1')
time.sleep(5)
print("DateTime " + time.strftime("%c"))
print('WBC2')
time.sleep(5)
print("DateTime " + time.strftime("%c"))
print('WBC3')
time.sleep(5)
print("DateTime " + time.strftime("%c"))
print('DYE')
time.sleep(5)
print("DateTime " + time.strftime("%c"))
"""
