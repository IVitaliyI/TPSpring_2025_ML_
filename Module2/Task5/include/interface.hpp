#pragma once
#ifndef INTERFACE_HPP
#define INTERFACE_GPP

typedef unsigned char byte;
#define interface struct

interface IInputStream {
	// Возвращает false, если поток закончился
	virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
	virtual void Write(byte value) = 0;
};

#endif // INTERFACE_GPP
