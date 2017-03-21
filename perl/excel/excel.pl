#!/usr/bin/perl -w
use Spreadsheet::Read;  
use Excel::Writer::XLSX;  
  
my $workbook = Excel::Writer::XLSX->new('b.xlsx');  
my $worksheet = $workbook->add_worksheet();  
  
my $format1 = $workbook->add_format(  
        valign => 'vcenter',
        align  => 'center', 
        bg_color => 'red', 
        color => 'green'  
    );  
  
$worksheet->write( 1,1, 'try fromat',$format1 );  
  
$workbook->close();
