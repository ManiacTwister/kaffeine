/*
 * dvbchannel.h
 *
 * Copyright (C) 2007-2008 Christoph Pfister <christophpfister@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef DVBCHANNEL_H
#define DVBCHANNEL_H

#include <QSharedData>
#include <QString>

class DvbCTransponder;
class DvbSTransponder;
class DvbTTransponder;
class AtscTransponder;

class DvbTransponderBase : public QSharedData
{
public:
	enum TransmissionType
	{
		DvbC = 0,
		DvbS = 1,
		DvbT = 2,
		Atsc = 3
	};

	enum FecRate
	{
		FecNone = 0,
		Fec1_2 = 1,
		Fec2_3 = 2,
		Fec3_4 = 3,
		Fec4_5 = 4,
		Fec5_6 = 5,
		Fec6_7 = 6,
		Fec7_8 = 7,
		Fec8_9 = 8,
		FecAuto = 9,
		FecRateMax = FecAuto
	};

	DvbTransponderBase() { }
	virtual ~DvbTransponderBase() { }

	virtual TransmissionType getTransmissionType() const = 0;

	virtual const DvbCTransponder *getDvbCTransponder() const
	{
		return NULL;
	}

	virtual const DvbSTransponder *getDvbSTransponder() const
	{
		return NULL;
	}

	virtual const DvbTTransponder *getDvbTTransponder() const
	{
		return NULL;
	}

	virtual const AtscTransponder *getAtscTransponder() const
	{
		return NULL;
	}
};

class DvbCTransponder : public DvbTransponderBase
{
public:
	enum Modulation
	{
		Qam16 = 0,
		Qam32 = 1,
		Qam64 = 2,
		Qam128 = 3,
		Qam256 = 4,
		ModulationAuto = 5,
		ModulationMax = ModulationAuto
	};

	TransmissionType getTransmissionType() const
	{
		return DvbC;
	}

	const DvbCTransponder *getDvbCTransponder() const
	{
		return this;
	}

	int frequency; // Hz
	int symbolRate; // symbols per second
	Modulation modulation;
	FecRate fecRate;
};

class DvbSTransponder : public DvbTransponderBase
{
public:
	enum Polarization
	{
		Horizontal = 0,
		Vertical = 1,
		CircularLeft = 2,
		CircularRight = 3,
		PolarizationMax = CircularRight
	};

	TransmissionType getTransmissionType() const
	{
		return DvbS;
	}

	const DvbSTransponder *getDvbSTransponder() const
	{
		return this;
	}

	int frequency; // kHz
	Polarization polarization;
	int symbolRate; // symbols per second
	FecRate fecRate;
};

class DvbTTransponder : public DvbTransponderBase
{
public:
	enum Bandwidth
	{
		Bandwidth6Mhz = 0,
		Bandwidth7Mhz = 1,
		Bandwidth8Mhz = 2,
		BandwidthAuto = 3,
		BandwidthMax = BandwidthAuto
	};

	enum Modulation
	{
		Qpsk = 0,
		Qam16 = 1,
		Qam64 = 2,
		ModulationAuto = 3,
		ModulationMax = ModulationAuto
	};

	enum TransmissionMode
	{
		TransmissionMode2k = 0,
		TransmissionMode8k = 1,
		TransmissionModeAuto = 2,
		TransmissionModeMax = TransmissionModeAuto
	};

	enum GuardInterval
	{
		GuardInterval1_4 = 0,
		GuardInterval1_8 = 1,
		GuardInterval1_16 = 2,
		GuardInterval1_32 = 3,
		GuardIntervalAuto = 4,
		GuardIntervalMax = GuardIntervalAuto
	};

	enum Hierarchy
	{
		HierarchyNone = 0,
		Hierarchy1 = 1,
		Hierarchy2 = 2,
		Hierarchy4 = 3,
		HierarchyAuto = 4,
		HierarchyMax = HierarchyAuto
	};

	TransmissionType getTransmissionType() const
	{
		return DvbT;
	}

	const DvbTTransponder *getDvbTTransponder() const
	{
		return this;
	}

	int frequency; // Hz
	Bandwidth bandwidth;
	Modulation modulation;
	FecRate fecRateHigh; // high priority stream
	FecRate fecRateLow; // low priority stream
	TransmissionMode transmissionMode;
	GuardInterval guardInterval;
	Hierarchy hierarchy;
};

class AtscTransponder : public DvbTransponderBase
{
public:
	enum Modulation
	{
		Qam64 = 0,
		Qam256 = 1,
		Vsb8 = 2,
		Vsb16 = 3,
		ModulationAuto = 4,
		ModulationMax = ModulationAuto
	};

	TransmissionType getTransmissionType() const
	{
		return Atsc;
	}

	const AtscTransponder *getAtscTransponder() const
	{
		return this;
	}

	int frequency; // Hz
	Modulation modulation;
};

class DvbTransponder : public QExplicitlySharedDataPointer<const DvbTransponderBase>
{
public:
	explicit DvbTransponder(const DvbTransponderBase *transponder = NULL) :
		QExplicitlySharedDataPointer<const DvbTransponderBase>(transponder) { }
	~DvbTransponder() { }
};

class DvbChannelBase
{
public:
	DvbChannelBase() : number(-1), networkId(-1), transportStreamId(-1), serviceId(-1),
		pmtPid(-1), videoPid(-1), audioPid(-1), scrambled(false) { }
	~DvbChannelBase() { }

	QString name;
	int number;

	QString source;
	int networkId; // may be -1 (not present)
	int transportStreamId; // may be -1 (not present)
	int serviceId;

	int pmtPid;
	int videoPid; // may be -1 (not present)
	int audioPid; // may be -1 (not present)

	bool scrambled;

	DvbTransponder transponder;
};

class DvbChannel : public DvbChannelBase, public QSharedData
{
public:
	DvbChannel() { }
	explicit DvbChannel(const DvbChannelBase &channel) : DvbChannelBase(channel) { }
	~DvbChannel() { }
};

class DvbSharedChannel : public QSharedDataPointer<DvbChannel>
{
public:
	explicit DvbSharedChannel(DvbChannel *channel = NULL) :
		QSharedDataPointer<DvbChannel>(channel) { }
	explicit DvbSharedChannel(const DvbChannelBase &channel) :
		QSharedDataPointer<DvbChannel>(new DvbChannel(channel)) { }
	~DvbSharedChannel() { }
};

class DvbLineReader
{
public:
	explicit DvbLineReader(const QString &line_) : line(line_), pos(0), valid(true) { }
	~DvbLineReader() { }

	bool isValid() const
	{
		return valid;
	}

	template<typename T> T readEnum(T maxValue)
	{
		int value = readInt();

		if (value > maxValue) {
			valid = false;
		}

		return static_cast<T>(value);
	}

	int readInt(bool allowEmpty = false);
	QString readString();

	DvbCTransponder *readDvbCTransponder();
	DvbSTransponder *readDvbSTransponder();
	DvbTTransponder *readDvbTTransponder();
	AtscTransponder *readAtscTransponder();

	DvbChannel *readChannel();

private:
	QString line;
	int pos;
	bool valid;
};

class DvbLineWriter
{
public:
	QString getLine();

	void writeInt(int value);
	void writeString(const QString &string);

	void writeTransponder(const DvbCTransponder *transponder);
	void writeTransponder(const DvbSTransponder *transponder);
	void writeTransponder(const DvbTTransponder *transponder);
	void writeTransponder(const AtscTransponder *transponder);

	void writeChannel(const DvbChannel *channel);

private:
	QString line;
};

#endif /* DVBCHANNEL_H */
