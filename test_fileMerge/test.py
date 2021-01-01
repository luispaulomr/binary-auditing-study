import os
import glob

fileSplit_exe = "C:\\Users\\IZZY\\source\\repos\\binary-auditing-study\\Release\\3.1.exe"
fileMerge_exe = "C:\\Users\\IZZY\\source\\repos\\binary-auditing-study\\Release\\3.1.2.exe"

def GenerateCommand(_list):
	
	ret = ""
	
	for l in _list:
	
		ret += l + " "

	return ret
	
def CheckGeneratedFileSizesAgainstOriginalFile(test_bigFile, test_baseName):

	size_bigFile = os.path.getsize(test_bigFile)
	
	size_splittedFiles = 0
	
	list_splittedFiles = glob.glob(test_baseName + "*")
	
	for file in list_splittedFiles:
		
		size_splittedFiles = size_splittedFiles + os.path.getsize(file)
			
	if size_splittedFiles != size_bigFile:
	
		print("File sizes differ!")
	
	else:
	
		print("File sizes are OK!")
	
def test_1():

	test_bigFile = "test.pdf"
	test_fileName = "test_1.pdf"
	test_baseName = "test_1_"
	test_chunkSize = "50000"
	test_bufferSize = "4096"
	test_debugMode = "0"

	# TEST_1: fileSplit
	split_command = []
	split_command.append(fileSplit_exe)
	# bigFile
	split_command.append(test_bigFile)
	# baseName
	split_command.append(test_baseName)
	# chunkSize
	split_command.append(test_chunkSize)
	# bufferSize
	split_command.append(test_bufferSize)
	# debugMode
	split_command.append(test_debugMode)
	
	# TEST_1: fileMerge
	merge_command = []
	merge_command.append(fileMerge_exe)
	# fileName
	merge_command.append(test_fileName)
	# baseName
	merge_command.append(test_baseName)
	# debugMode
	merge_command.append(test_debugMode)

	# run fileSplit
	command = GenerateCommand(split_command)
	print("Running command " + command)
	os.system(command)
	
	CheckGeneratedFileSizesAgainstOriginalFile(test_bigFile, test_baseName)

	command = GenerateCommand(merge_command)
	print("Running command " + command)
	os.system(command)

	#run diff command
	os.system("FC /B " + test_bigFile + " " + test_fileName)
	
def test_2():

	test_bigFile = "test.pdf"
	test_fileName = "test_2.pdf"
	test_baseName = "test_2_"
	test_chunkSize = "50000"
	test_bufferSize = "50050"
	test_debugMode = "0"

	# TEST_1: fileSplit
	split_command = []
	split_command.append(fileSplit_exe)
	# bigFile
	split_command.append(test_bigFile)
	# baseName
	split_command.append(test_baseName)
	# chunkSize
	split_command.append(test_chunkSize)
	# bufferSize
	split_command.append(test_bufferSize)
	# debugMode
	split_command.append(test_debugMode)
	
	# TEST_1: fileMerge
	merge_command = []
	merge_command.append(fileMerge_exe)
	# fileName
	merge_command.append(test_fileName)
	# baseName
	merge_command.append(test_baseName)
	# debugMode
	merge_command.append(test_debugMode)

	# run fileSplit
	command = GenerateCommand(split_command)
	print("Running command " + command)
	os.system(command)
	
	CheckGeneratedFileSizesAgainstOriginalFile(test_bigFile, test_baseName)

	command = GenerateCommand(merge_command)
	print("Running command " + command)
	os.system(command)

	#run diff command
	os.system("FC /B " + test_bigFile + " " + test_fileName)
	
def test_3():

	test_bigFile = "test.pdf"
	test_fileName = "test_3.pdf"
	test_baseName = "test_3_"
	test_chunkSize = "50000"
	test_bufferSize = "50000"
	test_debugMode = "0"

	# TEST_1: fileSplit
	split_command = []
	split_command.append(fileSplit_exe)
	# bigFile
	split_command.append(test_bigFile)
	# baseName
	split_command.append(test_baseName)
	# chunkSize
	split_command.append(test_chunkSize)
	# bufferSize
	split_command.append(test_bufferSize)
	# debugMode
	split_command.append(test_debugMode)
	
	# TEST_1: fileMerge
	merge_command = []
	merge_command.append(fileMerge_exe)
	# fileName
	merge_command.append(test_fileName)
	# baseName
	merge_command.append(test_baseName)
	# debugMode
	merge_command.append(test_debugMode)

	# run fileSplit
	command = GenerateCommand(split_command)
	print("Running command " + command)
	os.system(command)
	
	CheckGeneratedFileSizesAgainstOriginalFile(test_bigFile, test_baseName)

	command = GenerateCommand(merge_command)
	print("Running command " + command)
	os.system(command)

	#run diff command
	os.system("FC /B " + test_bigFile + " " + test_fileName)


test_file = "test.pdf"
test_basename = "test_2_"
test_chunksize = "50000"
test_buffersize = "50001"
test_debugmode = "1"
test_mergedfile = "test_2.bin"

test_file = "test.pdf"
test_basename = "test_3_"
test_chunksize = "50000"
test_buffersize = "50000"
test_debugmode = "1"
test_mergedfile = "test_3.bin"

test_1()
test_2()
test_3()

