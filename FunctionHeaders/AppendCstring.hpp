#ifndef _APPENDCSTRING_HEADER
#define _APPENDCSTRING_HEADER

#include <cstring>

namespace AppendCstring {

	static char* AppendCstring(const char* Source, const char* Appendant) {

		const size_t SourceLen = std::strlen(Source);
		const size_t AppendantLen = std::strlen(Appendant);

		char* Concat = new char[SourceLen + AppendantLen + 1];

		std::memcpy(Concat, Source, SourceLen);
		std::memcpy(Concat + SourceLen, Appendant, AppendantLen);
		Concat[SourceLen + AppendantLen] = '\0';

		return Concat;
	}
	static char* AppendCstringFree(char* Source, const char* Appendant) {

		char* Concat = AppendCstring::AppendCstring(Source, Appendant);
		delete[] Source;

		return Concat;
	}

	static char* AppendCharacter(const char* Source, char Appendant) {
		const size_t SourceLength = std::strlen(Source);

		char* Concat = new char[SourceLength + 2];
		std::memcpy(Concat, Source, SourceLength);

		Concat[SourceLength] = Appendant;
		Concat[SourceLength + 1] = '\0';

		return Concat;
	}
	static char* AppendCharacterFree(char* Source, char Appendant) {
		const size_t SourceLength = std::strlen(Source);

		char* Concat = new char[SourceLength + 2];
		std::memcpy(Concat, Source, SourceLength);
		delete[] Source;

		Concat[SourceLength] = Appendant;
		Concat[SourceLength + 1] = '\0';

		return Concat;
	}
}

#endif