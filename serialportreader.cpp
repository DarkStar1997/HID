#include "serialportreader.h"
#include <iostream>
#include <QCoreApplication>
#include <string>
#include <math.h>
#include <vector>
#include <chrono>
#include <fstream>

//int val[3]={0};
std::vector<int> val(6, 0);
int valCount=0;
int dx, dy, dz, old_x, old_y, old_z, x, y, z, potdefx, potdefy;
bool invert=false;
int butts[4];
double angdef=0.0;
Display* dpy=XOpenDisplay(NULL);
int scr=XDefaultScreen(dpy);
Window root_window=XRootWindow(dpy, scr);
int max_y=DisplayHeight(dpy, scr);
int max_x=DisplayWidth(dpy, scr);
int count=0;

std::chrono::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}
double span(std::chrono::steady_clock::time_point t1, std::chrono::steady_clock::time_point t2)
{
    std::chrono::duration<double> diff=t2-t1;
    return diff.count();
}

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent) :
    QObject(parent),
    m_serialPort(serialPort),
    m_standardOutput(stdout)
{
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortReader::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortReader::handleError);
    connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);
    old_x = max_x / 2; old_y = max_y / 2;
    dx = 0; dy = 0;
    //m_timer.start(5000);
}

void keyPress(std::string str)
{
    KeyCode modcode = XKeysymToKeycode(dpy, XStringToKeysym(str.data()));
    XTestFakeKeyEvent(dpy, modcode, False, 0);
    XFlush(dpy);
    usleep(50);
    XTestFakeKeyEvent(dpy, modcode, True, 0);
    XFlush(dpy);
    usleep(50);
    XTestFakeKeyEvent(dpy, modcode, False, 0);
    XFlush(dpy);
    usleep(50);
}

void rightClick()
{
    XTestFakeButtonEvent(dpy, 3, True, 0);
    XFlush(dpy);
    XTestFakeButtonEvent(dpy, 3, False, 0);
    XFlush(dpy);
    usleep(50);
}

void rightClick1()
{
    std::ofstream out; out.open("exe.txt");
    out << 2 << "\n0\n0\n";
    out.close();
    system("python3 mouse.py < exe.txt");
}

void leftClick()
{
    XTestFakeButtonEvent(dpy, 2, True, 0);
    XFlush(dpy);
    XTestFakeButtonEvent(dpy, 2, False, 0);
    XFlush(dpy);
    usleep(50);
}

void leftClick1()
{
    std::ofstream out; out.open("exe.txt");
    out << 1 << "\n0\n0\n";
    out.close();
    system("python3 mouse.py < exe.txt");
}

void moveTo(int x, int y)
{
    std::ofstream out; out.open("exe.txt");
    out << 0 << "\n" << x << "\n" << y << '\n';
    out.close();
    system("python3 mouse.py < exe.txt");
}

void SerialPortReader::handleReadyRead()
{
    std::string dem1;
    dem1=(m_serialPort->readLine()).toStdString();
    //std::cout<<dem1<<'\n';
    for(int i=0;i<dem1.length();i++)
    {
        char ch=dem1[i];
        if(ch>='0' && ch<='9')
        {
            val[valCount]=val[valCount]*10+(ch-'0');
        }
        else if(ch == 'z')
        {
            valCount=(valCount+1)%6;
        }
        else if(ch == 'i')
        {
            for(auto& i : val)
                i -= 130000;
            //Insert your code here
            x=(val[0]/400)*400;
            y=(val[1]/800)*800;
            z=(val[2]/400)*400;
            int butt=val[3];
            bool prev_act=butts[0]&&butts[1]&&butts[2]&&butts[3];

            butts[3]=butt/1000;
            butt%=1000;
            butts[2]=butt/100;
            butt%=100;
            butts[1]=butt/10;
            butt%=10;
            butts[0]=butt;

            int potx=val[4],poty=val[5];
            // buttons assigned
            std::cout << butts[0] << butts[1] << butts[2] << butts[3] << "\n";
            if(butts[0] && butts[1] && butts[2] && butts[3])
            {
                if(!prev_act)
                {
                    invert=!invert;
                }
                std::cout << "ALL BUTTONS PRESSED" << "\n";
            }

            if(butts[0] && butts[1])
            {
                keyPress("r");
            }

            if(butts[1] && butts[2])
            {
                keyPress("e");
            }

            if(butts[0])
            {
                std::cout << "Button 1 pressed\n";
                //keyPress("r");
                rightClick();
            }
            if(butts[1])
            {
                std::cout << "Button 2 pressed\n";
                //keyPress("e");
                leftClick();
            }
            if(butts[2])
            {
                std::cout << "Button 3 pressed\n";
                keyPress("w");
            }
            if(butts[3])
            {
                std::cout << "Button 4 pressed\n";
                keyPress("q");
            }

            if(x < 15000)
            {
                std::cout << "Moving left\n";
                keyPress("Left");
                //leftClick();
            }
            else if(x > 17000)
            {
                std::cout << "Moving rightt\n";
                //rightClick();
                keyPress("Right");
            }
            if(y < 16000)
            {
                std::cout << "Moving front\n";
                keyPress("Up");
            }
            else if(y > 19000)
            {
                std::cout << "Moving back\n";
                keyPress("Down");
            }


            /*if(butts[0]==1)
            {
                potdefx=max_x/2-potx;
                potdefy=max_y/2-poty;
                angdef=atan((double)(poty-515)/(potx-507)); //*180/3.14;
                double mag=sqrt(potdefx*potdefx+potdefy*potdefy);


                //std::cout<< "default angle= " << angdef*180/3.14 << " potdefx= " << potdefx << " potdefy= " << potdefy << "\n";
                //butts[0]=0;
            }*/
            /*
            potx=(int)(1.0*(potx-512)*cos(angdef)+1.0*poty*sin(angdef));
            poty=(int)(1.0*(poty-512)*cos(angdef)-1.0*potx*sin(angdef));

            */


            if(invert)
            {
                int temp=potx;
                potx=(poty-512)*max_x/1024 + max_x/2;
                poty=-(temp-512)*max_y/1024 + max_y/2;
            }
            else
            {
                potx=-(potx-512)*max_x/1024 + max_x/2;
                poty=-(poty-512)*max_y/1024 + max_y/2;
            }



            //std::cout<< x <<" "<< y <<" "<< z <<" "<<val[3] << " " << potx << " " << poty << " angdef= " << angdef << " potdefy= " << potdefy <<'\n';
            std::cout<< x <<" "<< y <<" "<< z <<" "<<val[3] << " " << potx << " " << poty << '\n';

            int dx=abs(old_x-x),dy=abs(old_y-y),dz=abs(old_z-z);
            //if(dx>5 || dy>5)
            {
                //XWarpPointer(dpy, 0L, root_window, 0, 0, 0, 0, (int)(x*5.0/100), (int)(y*5.0/100) );
                XWarpPointer(dpy, 0L, root_window, 0, 0, 0, 0, (int)(potx), (int)(poty) );
                XFlush(dpy);
                usleep(50);
                //moveTo(potx, poty);
            }

            old_x=x;
            old_y=y;
            old_z=z;

            val = {0, 0, 0, 0, 0, 0};
            valCount = 0;
        }
    }
}

void SerialPortReader::handleTimeout()
{
    if (m_readData.isEmpty()) {
        m_standardOutput << QObject::tr("No data was currently available "
                                        "for reading from port %1")
                            .arg(m_serialPort->portName())
                         << endl;
    } else {
        m_standardOutput << QObject::tr("Data successfully received from port %1")
                            .arg(m_serialPort->portName())
                         << endl;
        m_standardOutput << m_readData << endl;
    }

    QCoreApplication::quit();
}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading "
                                        "the data from port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                         << endl;
        QCoreApplication::exit(1);
    }
}
