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

#include "alp_getopts.h"

#include <iostream> // quitar. para depurar

#include "alp_string.h"
#include "alp_cast.h"

namespace alp{

Getopts_error::Getopts_error(const std::string& help,
                                                   const std::string& msg)
    : runtime_error{alp::as_str() << "Error: " << msg << '\n'
                                  << help << '\n'}
{
    }


std::vector<std::string> Getopts::cmd_line2vector(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i)
	args.push_back(argv[i]);

    return args;
}

std::pair<const std::string&, Getopts::vble_option&>
			Getopts::find_string_option(const std::string& opt)
{
    auto p = str_opts_.find(opt);
    if (p == str_opts_.end())
        throw Getopts_error{help_, alp::as_str()
                                         << "Unknown option " << opt};
                                         

    return {p->first, p->second};
}


std::pair<char, Getopts::vble_option&>
			    Getopts::find_char_option(char opt)
{
    auto p = char_opts_.find(opt);
    if (p == char_opts_.end())
        throw Getopts_error{help_, alp::as_str()
                                         << "Unknown option " << opt};
                                         
    return {p->first, p->second};
}


// parse: -char_options [valor]
// return: iterador al siguiente elemento a parsear
std::vector<std::string>::iterator 
	Getopts::parse_char_option(std::vector<std::string>::iterator a,
                             std::vector<std::string>::iterator args_end)
// [[ expects: (*a)[0] == '-' and (*a)[1] != '-' ]]
{
    if (a->size() == 2)	// *a = -c
	return parse_multichar_option(a, args_end);

    else 
	return parse_one_char_option(a, args_end);
}

// parse: -abcd 
// *a = -abcd..., tiene varias opciones todas de tipo bool
// return: iterador al siguiente elemento a parsear
std::vector<std::string>::iterator 
	Getopts::parse_one_char_option(std::vector<std::string>::iterator a,
                             std::vector<std::string>::iterator args_end)
{
    for (size_t i = 1; i < a->size(); ++i){

        auto [c_opt, opt] = find_char_option((*a)[i]);

        if (std::holds_alternative<bool*>(opt))
            *std::get<bool*>(opt) = true;

        else
            throw Getopts_error{
                help_,
                as_str() << "La opción -" << c_opt
                         << " no es de tipo booleana. Se esperaba booleana."};
    }
    ++a;

    return a;
}

// parse: -c [valor]	<-- solo pasamos 1 caracter en *a
// return: iterador al siguiente elemento a parsear
std::vector<std::string>::iterator 
	Getopts::parse_multichar_option(std::vector<std::string>::iterator a,
                             std::vector<std::string>::iterator args_end)
{
    auto [c_opt, opt] = find_char_option((*a)[1]);

    if (std::holds_alternative<bool*>(opt))
	*std::get<bool*>(opt) = true;

    else{
	++a;
	if (a == args_end)
	    throw Getopts_error{help_, alp::as_str()
					 << "Opción -" << c_opt
					 << " no tiene argumento"};

	try{
	if (std::holds_alternative<int*>(opt))
	    *std::get<int*>(opt) = std::stoi(*a);

	else if (std::holds_alternative<unsigned int*>(opt))
	    *std::get<unsigned int*>(opt) = std::stoul(*a);

	else if (std::holds_alternative<float*>(opt))
	    *std::get<float*>(opt) = std::stof(*a);

	else
	    *std::get<std::string*>(opt) = *a;

	}catch(std::invalid_argument& e){
            throw Getopts_error{help_,
                                alp::as_str() << "El argumento de la opción -"
                                              << c_opt << " no es correcto."};
        }
    }

    ++a;
    return a;

}


// parse: --option [valor]
// return: iterador al siguiente elemento a parsear
std::vector<std::string>::iterator 
	Getopts::parse_string_option(std::vector<std::string>::iterator a,
                             std::vector<std::string>::iterator args_end)
// [[ expects: (*a)[0] == '-' and (*a)[1] == '-' ]]
{
    auto [str_opt, opt] = find_string_option(a->substr(2));

    if (std::holds_alternative<bool*>(opt))
	*std::get<bool*>(opt) = true;

    else{
	try{
	++a;
	if (a == args_end)
	    throw Getopts_error{help_, alp::as_str()
					 << "Opción --" << str_opt
					 << " no tiene argumento"};

	if (std::holds_alternative<int*>(opt))
	    *std::get<int*>(opt) = std::stoi(*a);

	else if (std::holds_alternative<unsigned int*>(opt))
	    *std::get<unsigned int*>(opt) = std::stoul(*a);

	else if (std::holds_alternative<float*>(opt))
	    *std::get<float*>(opt) = std::stof(*a);

	else
	    *std::get<std::string*>(opt) = *a;

	}catch(std::invalid_argument& e){
            throw Getopts_error{help_,
                                alp::as_str() << "El argumento de la opción -"
                                              << str_opt << " no es correcto."};
        }
    }

    ++a;
    return a;
    
}

std::vector<std::string>::iterator 
	Getopts::parse_option(std::vector<std::string>::iterator a,
                             std::vector<std::string>::iterator args_end)
// [[ expects: (*a)[0] == '-' ]]
{
    if (a->size() == 1)
	throw Getopts_error{help_,  // TODO: implementar opción -
			    "Opción - sin argumento. TODO: falta "
			    "implementar este caso"};

    if ((*a)[1] == '-') // --option
	return parse_string_option(a, args_end);

    else    // -option
	return parse_char_option(a, args_end);
}


std::vector<std::string> Getopts::parse(int argc, char* argv[])
{
    std::vector<std::string> args = cmd_line2vector(argc, argv);
    std::vector<std::string> resto;

    auto a = args.begin();
    while (a != args.end()){
	if ((*a)[0] == '-')
	    a = parse_option(a, args.end());

	else{
	    resto.push_back(*a);
	    ++a;
	}
    }

    if (num_min_args_ != -1 and
		        (resto.size() < narrow_cast<size_t>(num_min_args_)))
        throw Getopts_error{help_, alp::as_str()
				 << "Número de argumentos incorrecto."};

    return resto;
}


}// namespace
