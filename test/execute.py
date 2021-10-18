import os

basedir = os.path.dirname(__file__)
mpvcmd = os.path.join(basedir, "..\\build\\mpvcmd.exe")

inpath = "D:\\Downloads\\2016 - Florinda Bolkan - Those days with Lucio.mp4"

cmd = "%s \"%s\"" % (mpvcmd, inpath)

#print(cmd)
os.system(cmd)


