<?php

define('SKIP_SOURCECODE_GENERATION', FALSE);


define('GLUE_PATH', '/mnt/prj/glue_dev');
#define('GLUE_PATH', 'd:/p/prj/glue_dev');

$smarty_config = Array	(
							'handler'			=> 'file',
							'path_template'		=> 'template_smarty',
							'path_compile'		=> '/tmp',
							'db_template_type'	=> 1,
							'cache_dir'			=> '/tmp',
							'force_compile'		=> true,
							'caching'			=> false,
						);
define('BASE_PATH', '.');
define('PATH_HTTP', '.');
define('PATH_FILE', '.');
define('PATH_FILE_FILES', '../files');
define('PATH_FILE_PHP', '../php');
$glue_config = Array(
						'path_base_glue'	=> GLUE_PATH,
						'path_base'			=> BASE_PATH,
						'error_report'		=> 'screen',
						'path_modules'		=> '.',
					);
require_once(GLUE_PATH . '/glue.php');
$glue =& new Glue($glue_config);
$smarty =& $glue->load_module('smarty', $smarty_config);
if($smarty === false)
	die('Module "smarty" not loaded!');



// ****************************************************************************************************************************************************
// *** Init
// ****************************************************************************************************************************************************
set_time_limit(0);
$filepath_base = '/var/www/html/johan/';

// ****************************************************************************************************************************************************
// *** Editions
// ****************************************************************************************************************************************************
define('EDITION_LITE', 0);
define('EDITION_STANDARD', 500);
define('EDITION_FULL', 1000);
define('EDITION_EVAL', 65535);
$array_edition = Array(
							EDITION_LITE		=> 'lite',
							EDITION_STANDARD	=> 'standard',
							EDITION_FULL		=> 'full',
							EDITION_EVAL		=> 'eval',
						);
$filter_edition_standard = Array(
								'bltfast' => Array(
													'ck',
													'ck_cf',
													'tn',
													'ck_tn',
													'op',
													'ck_op',
													'ad',
													'ck_ad',
													'sb',
													'ck_sb',
													'oh',
													'ck_oh',
													'ol',
													'ck_ol',
												),
								'bltflip' => Array(
													'',
													'ck'
												),
						);

// ****************************************************************************************************************************************************
// *** Filter Combinations
// ****************************************************************************************************************************************************
$filter_combination = Array(
								'bltfast' => Array(
														Array('ck'),
														Array('ck', 'cf'),
														Array('tn'),
														Array('ck', 'tn'),
														Array('cl'),
														Array('ck', 'cl'),
														Array('op'),
														Array('ck', 'op'),
														Array('ck', 'cf', 'op'),
														Array('tn', 'op'),
														Array('ck', 'tn', 'op'),
														Array('cl', 'op'),
														Array('ck', 'cl', 'op'),
														Array('ad'),
														Array('ck', 'ad'),
														Array('ck', 'cf', 'ad'),
														Array('tn', 'ad'),
														Array('ck', 'tn', 'ad'),
														Array('cl', 'ad'),
														Array('ck', 'cl', 'ad'),
														Array('op', 'ad'),
														Array('ck', 'op', 'ad'),
														Array('ck', 'cf', 'op', 'ad'),
														Array('tn', 'op', 'ad'),
														Array('ck', 'tn', 'op', 'ad'),
														Array('cl', 'op', 'ad'),
														Array('ck', 'cl', 'op', 'ad'),
														Array('sb'),
														Array('ck', 'sb'),
														Array('ck', 'cf', 'sb'),
														Array('tn', 'sb'),
														Array('ck', 'tn', 'sb'),
														Array('cl', 'sb'),
														Array('ck', 'cl', 'sb'),
														Array('op', 'sb'),
														Array('ck', 'op', 'sb'),
														Array('ck', 'cf', 'op', 'sb'),
														Array('tn', 'op', 'sb'),
														Array('ck', 'tn', 'op', 'sb'),
														Array('cl', 'op', 'sb'),
														Array('ck', 'cl', 'op', 'sb'),
														Array('ol'),
														Array('ck', 'ol'),
														Array('ck', 'cf', 'ol'),
														Array('tn', 'ol'),
														Array('ck', 'tn', 'ol'),
														Array('cl', 'ol'),
														Array('ck', 'cl', 'ol'),
														Array('ad', 'ol'),
														Array('ck', 'ad', 'ol'),
														Array('ck', 'cf', 'ad', 'ol'),
														Array('tn', 'ad', 'ol'),
														Array('ck', 'tn', 'ad', 'ol'),
														Array('cl', 'ad', 'ol'),
														Array('ck', 'cl', 'ad', 'ol'),
														Array('sb', 'ol'),
														Array('ck', 'sb', 'ol'),
														Array('ck', 'cf', 'sb', 'ol'),
														Array('tn', 'sb', 'ol'),
														Array('ck', 'tn', 'sb', 'ol'),
														Array('cl', 'sb', 'ol'),
														Array('ck', 'cl', 'sb', 'ol'),
														Array('oh'),
														Array('ck', 'oh'),
														Array('ck', 'cf', 'oh'),
														Array('tn', 'oh'),
														Array('ck', 'tn', 'oh'),
														Array('cl', 'oh'),
														Array('ck', 'cl', 'oh'),
														Array('ad', 'oh'),
														Array('ck', 'ad', 'oh'),
														Array('ck', 'cf', 'ad', 'oh'),
														Array('tn', 'ad', 'oh'),
														Array('ck', 'tn', 'ad', 'oh'),
														Array('cl', 'ad', 'oh'),
														Array('ck', 'cl', 'ad', 'oh'),
														Array('sb', 'oh'),
														Array('ck', 'sb', 'oh'),
														Array('ck', 'cf', 'sb', 'oh'),
														Array('tn', 'sb', 'oh'),
														Array('ck', 'tn', 'sb', 'oh'),
														Array('cl', 'sb', 'oh'),
														Array('ck', 'cl', 'sb', 'oh'),
												),
								'bltflip' => Array(
														Array(''),
														Array('ck'),
														Array('ck', 'cf'),
														Array('tn'),
														Array('ck', 'tn'),
														Array('cl'),
														Array('ck', 'cl'),
														Array('op'),
														Array('ck', 'op'),
														Array('ck', 'cf', 'op'),
														Array('tn', 'op'),
														Array('ck', 'tn', 'op'),
														Array('cl', 'op'),
														Array('ck', 'cl', 'op'),
														Array('ad'),
														Array('ck', 'ad'),
														Array('ck', 'cf', 'ad'),
														Array('tn', 'ad'),
														Array('ck', 'tn', 'ad'),
														Array('cl', 'ad'),
														Array('ck', 'cl', 'ad'),
														Array('op', 'ad'),
														Array('ck', 'op', 'ad'),
														Array('ck', 'cf', 'op', 'ad'),
														Array('tn', 'op', 'ad'),
														Array('ck', 'tn', 'op', 'ad'),
														Array('cl', 'op', 'ad'),
														Array('ck', 'cl', 'op', 'ad'),
														Array('sb'),
														Array('ck', 'sb'),
														Array('ck', 'cf', 'sb'),
														Array('tn', 'sb'),
														Array('ck', 'tn', 'sb'),
														Array('cl', 'sb'),
														Array('ck', 'cl', 'sb'),
														Array('op', 'sb'),
														Array('ck', 'op', 'sb'),
														Array('ck', 'cf', 'op', 'sb'),
														Array('tn', 'op', 'sb'),
														Array('ck', 'tn', 'op', 'sb'),
														Array('cl', 'op', 'sb'),
														Array('ck', 'cl', 'op', 'sb'),
														Array('ol'),
														Array('ck', 'ol'),
														Array('ck', 'cf', 'ol'),
														Array('tn', 'ol'),
														Array('ck', 'tn', 'ol'),
														Array('cl', 'ol'),
														Array('ck', 'cl', 'ol'),
														Array('ad', 'ol'),
														Array('ck', 'ad', 'ol'),
														Array('ck', 'cf', 'ad', 'ol'),
														Array('tn', 'ad', 'ol'),
														Array('ck', 'tn', 'ad', 'ol'),
														Array('cl', 'ad', 'ol'),
														Array('ck', 'cl', 'ad', 'ol'),
														Array('sb', 'ol'),
														Array('ck', 'sb', 'ol'),
														Array('ck', 'cf', 'sb', 'ol'),
														Array('tn', 'sb', 'ol'),
														Array('ck', 'tn', 'sb', 'ol'),
														Array('cl', 'sb', 'ol'),
														Array('ck', 'cl', 'sb', 'ol'),
														Array('oh'),
														Array('ck', 'oh'),
														Array('ck', 'cf', 'oh'),
														Array('tn', 'oh'),
														Array('ck', 'tn', 'oh'),
														Array('cl', 'oh'),
														Array('ck', 'cl', 'oh'),
														Array('ad', 'oh'),
														Array('ck', 'ad', 'oh'),
														Array('ck', 'cf', 'ad', 'oh'),
														Array('tn', 'ad', 'oh'),
														Array('ck', 'tn', 'ad', 'oh'),
														Array('cl', 'ad', 'oh'),
														Array('ck', 'cl', 'ad', 'oh'),
														Array('sb', 'oh'),
														Array('ck', 'sb', 'oh'),
														Array('ck', 'cf', 'sb', 'oh'),
														Array('tn', 'sb', 'oh'),
														Array('ck', 'tn', 'sb', 'oh'),
														Array('cl', 'sb', 'oh'),
														Array('ck', 'cl', 'sb', 'oh'),
												),

					);


$routine_function = Array(
							'bltfast'		=> 'ERdrBltFast',
							'bltflip'		=> 'ERdrBltFlip',
							'bltfull'		=> 'ERdrBltFull',
						);

// ****************************************************************************************************************************************************
// *** Function Map
// ****************************************************************************************************************************************************
$map = Array(
				'default'	=> 0,
				'ck'		=> 1,
				'cf'		=> 2,
				'tn'		=> 4,
				'cl'		=> 6,		# (combinatie cf/tn)
				'op'		=> 8,
				'ad'		=> 16,
				'sb'		=> 32,
				'oh'		=> 64,
				'ol'		=> 128,
		);
$funcmap = Array();


if(!SKIP_SOURCECODE_GENERATION)
{
	$output = '';
	foreach($filter_combination as $routine=>$filter)
	{
		foreach(Array(8, 12, 16, 32) as $colordepth)
		{
			foreach($filter as $filter_array)
			{
				$sum = 0;
				foreach($filter_array as $key)
					if($key != '')
						$sum += $map[$key];
				$funcmap[$sum] = $routine_function[$routine] . $colordepth . '_' . join('_', $filter_array);
			}
	
			$output .= 'const void *erdr_' . $routine . '_fptrX[] =' . NEWLINE . '{' . NEWLINE;
			for($i=0; $i<192; $i++)
			{
				$output .= "\t";
				$output .= isset($funcmap[$i]) ? $funcmap[$i] : 'NULL';
				if($i < 191)
					$output .= ',';
				if(isset($_GET['debug']) && $_GET['debug'])
				{
					$output .= '		// ' . $i;
					if($i > 0)
					{
						$output .= ' ';
						foreach($map as $key=>$value)
							$output .= $i & $value ? $key . '/' : '';
						$output = substr($output, 0, -1);
					}
				}
				$output .= NEWLINE;
			}
			$output .= '};' . NEWLINE . NEWLINE;
		}
	}
	echo "<PRE>$output</PRE>";
}


// ****************************************************************************************************************************************************
// *** Filter Functions
// ****************************************************************************************************************************************************
$cache = Array();
foreach($array_edition as $edition => $edition_str)
{
		if(!SKIP_SOURCECODE_GENERATION)
		{
			print"<PRE>\n";
			$filepath = $filepath_base . $edition_str . '/';
		
			$separate_files = Array();
			$complete_output = '';
			foreach($filter_combination as $routine=>$filter)
			{
					foreach(Array(8, 12, 16, 32) as $colordepth)
					{
						foreach($filter as $filter_array)
						{
							### Set template variables
							foreach(Array('ck', 'cf', 'cl', 'tn', 'ad', 'sb', 'op', 'oh', 'ol') as $var)
								$smarty->assign($var, in_array($var, $filter_array) ? true : false);
							$filter_join = join('_', $filter_array);
							$smarty->assign('funcext', '_' . $filter_join);
	
							### Compile template
							ob_start();
							$smarty->display_template($routine . $colordepth);
							$code = ob_get_contents();
							ob_end_clean();
	
							# Reformat newlines and whitespace
							$code = preg_replace("/\r/", '', $code);
							$code = preg_replace("/\s+\n/", "\n", $code);
							$code = preg_replace("/\n+/", "\n", $code);
							$code = trim($code);
	
							if($edition == EDITION_LITE)
							{
								# Alle functies wegbutsen voor de LITE edition
								$clear_function = true;
							}
							else if($edition == EDITION_STANDARD)
							{
								### Alle functies wegbutsen die niet zijn gedefinieerd in $filter_edition_standard
								$clear_function = !in_array($filter_join, $filter_edition_standard[$routine]);
							}
							else
							{
								### Full edition krijgt alle functies (ook Eval)
								$clear_function = false;
							}
	
							if($clear_function)
							{
								$code = substr($code, 0, strpos($code, '{') + 1) . "\n\t" . 'return false;' . "\n" . '}';
							}
	
	
							# Output function
							echo"// $edition_str - $routine / $colordepth / $filter_join\n";
							#echo $code . "\n\n";
							#exit;
							$complete_output .= $code . "\n\n";
		
							// Write to CPP
							$filename = $routine_function[$routine] . $colordepth . '_' . join('_', $filter_array) . '.cpp';
							#print"<LI>$filename";
							$fh = fopen($filepath . $filename, 'w') or die("Can't write to $filepath$filename");
							if($fh)
							{
								fwrite($fh, '#include "defs.h"' . "\n\n" . $code . "\n\n");
								fclose($fh);
								$separate_files[] = $filename;
							}
						}
					}
			}
			print"</PRE>\n";
			
			
			// Write edgerender_libid.cpp
			$filename = 'edgerender_libid.cpp';
			$code =		'int edgerender_getlibid(void)' . "\n" .
						'{' . "\n" .
						'	return(' . $edition . ');' . "\n" .
						'}' . "\n";
			$fh = fopen($filepath . $filename, 'w') or die("Can't write to $filepath$filename");
			if($fh)
			{
				fwrite($fh, $code . "\n\n");
				fclose($fh);
				$separate_files[] = $filename;
			}
			$complete_output .= $code . "\n\n";
		

			// Write to Complete CPP
			$filename = '$edgerender.cpp';
			$fh = fopen($filepath . $filename, 'w') or die("Can't write to $filepath$filename");
			if($fh)
			{
				fwrite($fh, '#include "defs.h"' . "\n\n" . $complete_output . "\n\n");
				fclose($fh);
			}
		}



		// ****************************************************************************************************************************************************
		// *** compile.bat
		// ****************************************************************************************************************************************************
		$compile_tool = Array(
									'gp2x_devkit'			=> Array(
																		'compile'	=> '"p:\prj\games\sdk\devkitgp2x\bin\arm-linux-g++" -O3 -c "%filename%.cpp" -o "%outdir%/%filename%.o"',
																		'linker'	=> '"p:\prj\games\sdk\devkitgp2x\bin\arm-linux-ar" cr "%outdir%/edgerender.lib" %objfiles%',
																		'ranlib'	=> '"p:\prj\games\sdk\devkitgp2x\bin\arm-linux-ranlib" "%outdir%/edgerender.lib"',
																	),
									'gp2x_official'			=> Array(
																		'compile'	=> '"p:\prj\games\sdk\GP2XSDK\Tools\arm-gp2x-linux\bin\g++" -O3 -c "%filename%.cpp" -o "%outdir%/%filename%.o"',
																		'linker'	=> '"p:\prj\games\sdk\GP2XSDK\Tools\arm-gp2x-linux\bin\ar" cr "%outdir%/edgerender.lib" %objfiles%',
																		'ranlib'	=> '"p:\prj\games\sdk\GP2XSDK\Tools\arm-gp2x-linux\bin\ranlib" "%outdir%/edgerender.lib"',
																	),
									'symbian91'				=> Array(
																		'compile'	=> '"C:\Program Files\CSL Arm Toolchain\bin\arm-none-symbianelf-g++.exe" -O3 -fno-unit-at-a-time -Wall -Wno-ctor-dtor-privacy -Wno-unknown-pragmas -fexceptions -march=armv5t -mapcs -pipe -nostdinc -c -msoft-float -x c++ -o "%outdir%/%filename%.o" "%filename%.cpp"',
																		'linker'	=> '"C:\Program Files\CSL Arm Toolchain\bin\arm-none-symbianelf-ar.exe" cr "%outdir%/edgerender.lib" %objfiles%',
																	),
									'symbian61'				=> Array(
																		'compile'	=> '"C:\Symbian\6.1\Shared\EPOC32\gcc\bin\gcc.exe" -s -fomit-frame-pointer -O3 -march=armv4t -mthumb-interwork -pipe -c -nostdinc -Wall -Wno-ctor-dtor-privacy -Wno-unknown-pragmas -o "%outdir%/%filename%.o" "%filename%.cpp"',
																		'linker'	=> '"C:\Symbian\6.1\Shared\EPOC32\gcc\bin\ar.exe" cr "%outdir%/edgerender.lib" %objfiles%',
																	),
									'android_codesourcery'	=> Array(
																		'compile'	=> '"C:\Program Files\CodeSourcery\Sourcery G++ Lite\bin\arm-none-linux-gnueabi-g++.exe" -O3 -c "%filename%.cpp" -o "%outdir%/%filename%.o"',
																		'linker'	=> '"C:\Program Files\CodeSourcery\Sourcery G++ Lite\bin\arm-none-linux-gnueabi-ar.exe" cr "%outdir%/edgerender.lib" %objfiles%',
																		'ranlib'	=> '"C:\Program Files\CodeSourcery\Sourcery G++ Lite\bin\arm-none-linux-gnueabi-ranlib.exe" "%outdir%/edgerender.lib"',
																	),
									'linux'	=> Array(
																		'linux'     => 1,
																		'compile'	=> '/usr/bin/g++ -O3 -c "%filename%.cpp" -o "%outdir%/%filename%.o"',
																		'linker'	=> '/usr/bin/ar cr "%outdir%/edgerender.lib" %objfiles%',
																		'ranlib'	=> '/usr/bin/ranlib "%outdir%/edgerender.lib"',
																	),

									'iphone'	=> Array(
																		'linux'     => 1,
																		'compile'	=> '/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin9-g++-4.0.1 -O3 -c "%filename%.cpp" -o "%outdir%/%filename%.o"',
																		'linker'	=> '/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar cr "%outdir%/edgerender.lib" %objfiles%',
																	),

									'iphone_simulator'	=> Array(
																		'linux'     => 1,
																		'compile'	=> '/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/i686-apple-darwin9-g++-4.0.1 -O3 -c "%filename%.cpp" -o "%outdir%/%filename%.o"',
																		'linker'	=> '/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/ar cr "%outdir%/edgerender.lib" %objfiles%',
																	),
							);

		foreach($compile_tool as $platform => $tool)
		{
			
			if(!isset($tool['linux']))
				$tool['linux'] = false;
			
			print"<LI>$platform\n";
			$outdir = 'out_' . $platform;

			$gcc_compile = '';
			$gcc_compile .= ($tool['linux'] ? '#' : '@rem') . ' Verwijder oude lib' . NEWLINE;
			if($tool['linux'])
				$gcc_compile .= 'rm "' . $outdir . '/' . 'edgerender.lib"' . NEWLINE . NEWLINE;
			else
				$gcc_compile .= 'del "' . $outdir . '/' . 'edgerender.lib"' . NEWLINE . NEWLINE;

			$gcc_compile .= ($tool['linux'] ? '#' : '@rem') . ' Compile (per .cpp bestand)' . NEWLINE;
			foreach($separate_files as $filename)
			{
				#print"<LI>$platform/$filename";
				$filename = substr($filename, 0, -4);
				$command = $tool['compile'];
				$command = str_replace('%filename%', $filename, $command);
				$command = str_replace('%outdir%', $outdir, $command);
				$gcc_compile .= $command . NEWLINE;
			}
			$gcc_compile .= NEWLINE;


			$separate_files_copy = $separate_files;
			$chunk = 100;
			$gcc_compile .= ($tool['linux'] ? '#' : '@rem') . ' Linken (per ' . $chunk . ' .o bestanden)' . NEWLINE;
			foreach(range(0, ceil(count($separate_files_copy) / $chunk) - 1) as $index)
			{
				$slice = Array();
				foreach(array_splice($separate_files_copy, 0, $chunk) as $f)
				{
					$f = substr($f, 0, -4) . '.o';
					$slice[] = '"' . $outdir . '/' . $f . '"';
				}

				$command = $tool['linker'];
				$command = str_replace('%outdir%', $outdir, $command);
				$command = str_replace('%objfiles%', join(' ', $slice), $command);
				$gcc_compile .= $command . NEWLINE;
			}
			$gcc_compile .= NEWLINE;

			if(isset($tool['ranlib']))
			{
				$gcc_compile .= ($tool['linux'] ? '#' : '@rem') . ' Verkleinen lib' . NEWLINE;

				$command = $tool['ranlib'];
				$command = str_replace('%outdir%', $outdir, $command);
				$gcc_compile .= $command . NEWLINE;
			}

			$batchfile = 'compile_' . $platform . '.bat';
			print"<LI>Writing ". $filepath . $batchfile . "\n";
			$fh = fopen($filepath . $batchfile, 'w') or die("Can't write to $filepath$batchfile");
			if($fh)
			{
				fwrite($fh, $gcc_compile);
				fclose($fh);
			}
		}
}

?>