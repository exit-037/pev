#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "include/libpe.h"
#include "include/output.h"
#include "include/parser.h"
/* modules */
#include "modules/tls.h"

extern struct options config;

char *dec2bin(unsigned int dec, char *bin, int bits)
{
	// por Gabriel Duarte <confusosk8@gmail.com>
	int i;

	for(i=0; i<bits; i++)
		bin[bits - i - 1] = (dec & (0x1 << i)) ? '1' : '0';

	bin[bits] = '\0';

	return bin;
}

void print_optional_header(PE_FILE *pe)
{
	char s[50];
	int subsystem;
	
	static const char *subs_desc[] = {
	"Unknown subsystem",
	"System native",
	"Windows GUI",
	"Windows CLI",
	"Posix CLI",
	"Windows CE GUI",
	"EFI application",
	"EFI driver with boot",
	"EFI run-time driver",
	"EFI ROM",
	"XBOX"
};
	
	output("Optional/Image header", NULL);
	
	if (pe->optional_ptr->_32)
	{
		sprintf(s, "%#x (%s)", pe->optional_ptr->_32->Magic, "PE32");
		output("Magic number", s);
		
		sprintf(s, "%d", pe->optional_ptr->_32->MajorLinkerVersion);
		output("Linker major version", s);
		
		sprintf(s, "%d", pe->optional_ptr->_32->MinorLinkerVersion);
		output("Linker minor version", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfCode);
		output("Size of .text secion", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfInitializedData);
		output("Size of .data secion", s);
				
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfUninitializedData);
		output("Size of .bss secion", s);
				
		sprintf(s, "%#x", pe->optional_ptr->_32->AddressOfEntryPoint);
		output("Entrypoint", s);
				
		sprintf(s, "%#x", pe->optional_ptr->_32->BaseOfCode);
		output("Address of .text section", s);
				
		sprintf(s, "%#x", pe->optional_ptr->_32->BaseOfData);
		output("Address of .data section", s);
				
		sprintf(s, "%#x", pe->optional_ptr->_32->ImageBase);
		output("ImageBase", s);
				
		sprintf(s, "%#x", pe->optional_ptr->_32->SectionAlignment);
		output("Alignment of sections", s);
				
		sprintf(s, "%#x", pe->optional_ptr->_32->FileAlignment);
		output("Alignment factor", s);
				
		sprintf(s, "%d", pe->optional_ptr->_32->MajorOperatingSystemVersion);
		output("Major version of required OS", s);
				
		sprintf(s, "%d", pe->optional_ptr->_32->MinorOperatingSystemVersion);
		output("Minor version of required OS", s);
				
		sprintf(s, "%d", pe->optional_ptr->_32->MajorImageVersion);
		output("Major version of image", s);
		
		sprintf(s, "%d", pe->optional_ptr->_32->MinorImageVersion);
		output("Minor version of image", s);
		
		sprintf(s, "%d", pe->optional_ptr->_32->MajorSubsystemVersion);
		output("Major version of subsystem", s);
		
		sprintf(s, "%d", pe->optional_ptr->_32->MinorSubsystemVersion);
		output("Minor version of subsystem", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfImage);
		output("Size of image", s);
		
		sprintf(s, "%#d", pe->optional_ptr->_32->SizeOfHeaders);
		output("Size of headers", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->CheckSum);
		output("Checksum", s);
		
		subsystem = pe->optional_ptr->_32->Subsystem;
		sprintf(s, "%#x (%s)", subsystem, subsystem <= 10 ? subs_desc[subsystem] : "Unknown");
		output("Subsystem required", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->DllCharacteristics);
		output("DLL characteristics", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfStackReserve);
		output("Size of stack to reserve", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfStackCommit);
		output("Size of stack to commit", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfHeapReserve);
		output("Size of heap space to reserve", s);
		
		sprintf(s, "%#x", pe->optional_ptr->_32->SizeOfHeapCommit);
		output("Size of heap space to commit", s);
	}
}

void print_coff_header(IMAGE_COFF_HEADER *header)
{
	char s[50];
	char time[40];
	register int i, j;
	
	static const char *flags[] = {
	"base relocations stripped",
	"executable image",
	"line numbers removed (deprecated)",
	"local symbols removed (deprecated)",
	"aggressively trim (deprecated for Windows 2000 and later)",
	"can handle more than 2 GB addresses", "",
	"little-endian (deprecated)",
	"32-bit machine",
	"debugging information removed",
	"copy to swap if it's on removable media",
	"copy to swap if it's on network media",
	"system file",
	"DLL image",
	"uniprocessor machine",
	"big-endian (deprecated)"
	};
	
	output("COFF/File header", NULL);
	
	sprintf(s, "%#x %s", header->Machine, "INTEL");
	output("Machine", s);
	
	sprintf(s, "%d", header->NumberOfSections);
	output("Number of sections", s);
	
	strftime(time, 40, "%a, %d %b %Y %H:%M:%S UTC", gmtime((time_t *) & header->TimeDateStamp));
	sprintf(s, "%d (%s)", header->TimeDateStamp, time);
	output("Date/time stamp", s);

	sprintf(s, "%#x", header->PointerToSymbolTable);
	output("Symbol Table offset", s);
	
	sprintf(s, "%d", header->NumberOfSymbols);
	output("Number of symbols", s);
	
	sprintf(s, "%#x", header->SizeOfOptionalHeader);
	output("Size of optional header", s);
	
	sprintf(s, "%#x", header->Characteristics);
	output("Characteristics", s);
	
	for (i=1, j=0; i<0x8000; i<<=1, j++)
		if (header->Characteristics & i) output(NULL, (char*)flags[j]);
}

void print_dos_header(IMAGE_DOS_HEADER *header)
{
	char s[50];
	
	output("DOS Header", NULL);
	
	sprintf(s, "%#x (MZ)", header->e_magic);
	output("Magic number", s);
	
	sprintf(s, "%d", header->e_cblp);
	output("Bytes in last page", s);
	
	sprintf(s, "%d", header->e_cp);
	output("Pages in file", s);
		
	sprintf(s, "%d", header->e_crlc);
	output("Relocations", s);
		
	sprintf(s, "%d", header->e_cparhdr);
	output("Size of header in paragraphs", s);
		
	sprintf(s, "%d", header->e_minalloc);
	output("Minimum extra paragraphs", s);
		
	sprintf(s, "%d", header->e_maxalloc);
	output("Maximum extra paragraphs", s);
		
	sprintf(s, "%#x", header->e_ss);
	output("Initial (relative) SS value", s);
	
	sprintf(s, "%#x", header->e_sp);
	output("Initial SP value", s);
		
	sprintf(s, "%#x", header->e_ip);
	output("Initial IP value", s);
		
	sprintf(s, "%#x", header->e_cs);
	output("Initial (relative) CS value", s);
		
	sprintf(s, "%#x", header->e_lfarlc);
	output("Address of relocation table", s);
		
	sprintf(s, "%#x", header->e_ovno);
	output("Overlay number", s);
		
	sprintf(s, "%#x", header->e_oemid);
	output("OEM identifier", s);
		
	sprintf(s, "%#x", header->e_oeminfo);
	output("OEM information", s);
		
	sprintf(s, "%#x", header->e_lfanew);
	output("PE header offset", s);
}

int main(int argc, char *argv[])
{
	PE_FILE pe;
	FILE *fp;
	int i;
	
	parse_options(argc, argv); // opções
	
	for (i=argc-1, fp=NULL; i>0; i--)
	{
		fp = fopen(argv[i], "rb");
		if (fp)
			break;
	}
	
	if (fp == NULL)
		EXIT_WITH_ERROR("file not found or unreadable");
	
	pe_init(&pe, fp); // inicializa o struct pe
		
	if (!ispe(&pe))
		EXIT_WITH_ERROR("not a valid PE file");
		
	// dos header
	if (config.dos || config.all_headers)
	{
		IMAGE_DOS_HEADER dos;
		
		if (pe_get_dos(&pe, &dos))
			print_dos_header(&dos);
		else { EXIT_WITH_ERROR("unable to read DOS header"); }
	}
	
	// coff/file header
	if (config.coff || config.all_headers)
	{
		IMAGE_COFF_HEADER coff;
		
		if (pe_get_coff(&pe, &coff))
			print_coff_header(&coff);
		else { EXIT_WITH_ERROR("unable to read COFF file header"); }
	}
	
	// optional header
	if (config.opt || config.all_headers)
	{
		if (pe_get_optional(&pe))
			print_optional_header(&pe);
		else { EXIT_WITH_ERROR("unable to read Optional (Image) file header"); }
	}

	// libera a memória
	pe_clear(&pe);
	return 0;
}