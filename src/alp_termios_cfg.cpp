// Copyright (C) 2019-2020 Manuel Perez <manuel2perez@proton.me>
//
// This file is part of the ALP Library.
//
// ALP Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "alp_termios_cfg.h"

#include <string.h>	// memset
#include <iostream>

namespace alp{
// Abrimos el puerto indicado por tty_path
Termios_cfg::Termios_cfg() noexcept
{
    // inicializo a 0 toda la configuración
    memset(&cfg_, 0, sizeof(cfg_));
}

void Termios_cfg::print_as_hex(std::ostream& out) const 
{
    out << std::hex  << std::showbase
	<< "iflag: " << cfg_.c_iflag 
	<< "\noflag: " << cfg_.c_oflag
	<< "\ncflag: " << cfg_.c_cflag
	<< "\nlflag: " << cfg_.c_lflag;

//    out << "\ncflag: "; 
//    for (size_t i = 0; i < sizeof(termios::c_cc); ++i)
//	out << cfg_.c_cc[i] << ' ';

    out << '\n';


}


void Termios_cfg::print(std::ostream& out) const 
{
    print_iflag(out); out << '\n';
    print_oflag(out); out << '\n';
    print_cflag(out); out << '\n';
    print_lflag(out); out << '\n';
    print_ccflag(out);

}

// print_flag
static void pflag(std::ostream& out, bool b, const std::string& msg)
{
    if (b)
	out << "  true ";
    else
	out << "  false";

    out << ": " << msg << '\n';
}

void Termios_cfg::print_iflag(std::ostream& out) const 
{
    out << "Input modes (iflag)\n";
    
    pflag(out, cfg_.c_iflag & IGNBRK, "Ignore BREAK condition on input.");

    if (cfg_.c_iflag & IGNBRK)
	pflag(out, cfg_.c_iflag & BRKINT, "BREAK is ignored.");

    else
	pflag(out, cfg_.c_iflag & BRKINT, 
		    "BREAK causes the input and output queues to be flushed ...");


    pflag(out, cfg_.c_iflag & IGNPAR, "Ignore framing errors and parity errors.");

    if ((cfg_.c_iflag & INPCK) and !(cfg_.c_iflag & IGNPAR))
	pflag(out, cfg_.c_iflag & PARMRK, "Input bytes with parity or framing "
					  "errors are marked when passed to the program.");

    // TODO: ¿qué significa en el man de termios al hablar de PARMRK:
    //	     If  neither  IGNPAR  nor	PARMRK is set, read a character with a
    //	     parity error or framing error as \0.

    pflag(out, cfg_.c_iflag & INPCK, "Enable input parity checking.");
    pflag(out, cfg_.c_iflag & ISTRIP, "Strip off eighth bit.");
    pflag(out, cfg_.c_iflag & INLCR, "Translate NL to CR on input.");
    pflag(out, cfg_.c_iflag & IGNCR, "Ignore carriage return on input.");

    if (!(cfg_.c_iflag & IGNCR))
	pflag(out, cfg_.c_iflag & ICRNL, "Translate carriage return to newline on input .");

    pflag(out, cfg_.c_iflag & IUCLC, 
		"(not in POSIX) Map uppercase characters to lowercase on input.");

    pflag(out, cfg_.c_iflag & IXON, "Enable XON/XOFF flow control on output.");

    pflag(out, cfg_.c_iflag & IXANY, "(XSI)  Typing  any  character will restart stopped output.");
    pflag(out, cfg_.c_iflag & IXOFF, "Enable XON/XOFF flow control on input.");
    pflag(out, cfg_.c_iflag & IUTF8, "(not  in POSIX) Input is UTF8.");
}

void Termios_cfg::print_oflag(std::ostream& out) const 
{
    out << "Output modes(oflag)\n";

    pflag(out, cfg_.c_oflag & OPOST, "Enable implementation-defined output processing.");
    pflag(out, cfg_.c_oflag & OLCUC, "(not in POSIX) Map lowercase characters to uppercase on output.");
    pflag(out, cfg_.c_oflag & ONLCR, "(XSI) Map NL to CR-NL on output.");
    pflag(out, cfg_.c_oflag & OCRNL, "Map CR to NL on output.");
    pflag(out, cfg_.c_oflag & ONOCR, "Don't output CR at column 0.");
    pflag(out, cfg_.c_oflag & ONLRET, "Don't output CR.");
    pflag(out, cfg_.c_oflag & OFILL, "Send fill characters for a delay, rather than using a timed delay.");
    pflag(out, cfg_.c_oflag & VTDLY, "Vertical tab delay mask. Values are VT0 or VT1.");

}


void Termios_cfg::print_cflag(std::ostream& out) const 
{
    out << "Control modes (cflag)\n";

    auto csize = cfg_.c_cflag & CSIZE;
    out << "  CSIZE = ";
    if (csize == CS5) out << "CS5\n";
    else if (csize == CS6) out << "CS6\n";
    else if (csize == CS7) out << "CS7\n";
    else if (csize == CS8) out << "CS8\n";
    else out << "unknown!!!\n";

    pflag(out, cfg_.c_cflag & CSTOPB, "Set two stop bits, rather than one.");
    pflag(out, cfg_.c_cflag & CREAD , "Enable receiver.");
    pflag(out, cfg_.c_cflag & PARENB, "Enable parity generation on output and parity checking  for input.");
    pflag(out, cfg_.c_cflag & PARODD, "If set, then parity for input and output is odd; otherwise\n"
				      "         even parity is used.");
    pflag(out, cfg_.c_cflag & HUPCL , "Lower modem control lines after last process closes the device\n"
	                              "         (hang up).");
    pflag(out, cfg_.c_cflag & CLOCAL, "Ignore modem control lines.");



}


void Termios_cfg::print_lflag(std::ostream& out) const 
{
    out << "Local modes (lflag)\n";

    pflag(out, cfg_.c_lflag & ISIG,   "When any of the characters INTR, QUIT, SUSP, or DSUSP\n"
	                              "         are received, generate the corresponding signal.");
    pflag(out, cfg_.c_lflag & ICANON, "Enable canonical mode.");
    pflag(out, cfg_.c_lflag & ECHO, "Echo input characters.");

    if (cfg_.c_lflag & ICANON){
	pflag(out, cfg_.c_lflag & ECHOE, "The ERASE character erases the preceding input character, and WERASE erases the preceding word.");
	pflag(out, cfg_.c_lflag & ECHOK, "The KILL  character  erases  the  current line.");
	pflag(out, cfg_.c_lflag & ECHONL, "Echo the NL character even if ECHO is not set.");
    }

    pflag(out, cfg_.c_lflag & NOFLSH, "Disable flushing the input and output queues when\n"
				      "         generating signals for the INT, QUIT, and SUSP characters.");
    pflag(out, cfg_.c_lflag & TOSTOP, "Send the SIGTTOU signal to the process group of a\n"
				      "         background process which tries to write to its controlling terminal.");
    pflag(out, cfg_.c_lflag & IEXTEN, "Enable implementation-defined input processing ...");
  }


void Termios_cfg::print_ccflag(std::ostream& out) const 
{
    out << "Special characters (ccflag)\n";
    out << "VMIN  = " << (int) cfg_.c_cc[VMIN] << '\n';
    out << "VTIME = " << (int) cfg_.c_cc[VTIME] << '\n';
}



}// namespace


