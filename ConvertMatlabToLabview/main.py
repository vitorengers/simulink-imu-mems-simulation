# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import codecs
import sys

import csv
# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    # print(len("temp: 23.30 umi: 76.00 luz: 173"))

    # ####aaaaaa
    # with open("scc1_p1.txt", "r") as f:
    #     reader = csv.reader(f, delimiter="\t")
    #     d = list(reader)
    #
    # timestamp = []
    # initialTime = d[0][0]
    # degress = []
    #
    # time = 0.0
    # timestep = 1/2300.0
    # for i in range(len(d)):
    #     timestamp.append(time)
    #     degress.append(str(float(d[i][1])/3600))
    #     time += timestep
    #
    # with open("out_dps.txt", "w") as output:
    #     for i in range(len(degress)):
    #         output.write(str(timestamp[i]).replace(".", ",") + "\t" + degress[i].replace(".", ",") + "\n")
    # ####ESSE EH O BAO
    outFile = open("BNO/Msym_datasheet2_labview.txt", "w")
    count = 0
    with open("BNO/Msym_datasheet2.txt", "r") as f:
        reader = csv.reader(f, delimiter="\t")
        for line in reader:
            # if count <= 50000000:
            line2 = line[0][3:len(line[0])]
            outFile.write(str(line2).replace("   ", "\t").replace("  ", "\t") + "\n")
            # count = count + 1

        f.close()

    outFile.close()
    # print(count)
# ####ESSE EH O BAO ^^^
    # timestamp = []
    # initialTime = d[0][0]
    # degress = []
    #
    #
    # for i in range(len(d)):
    #     timestamp.append(str(float((int(d[i][0]) - int(initialTime)))/1000.0))
    #     degress.append(str(float(d[i][1])/3600))
    #
    # with open("out_dps.txt", "w") as output:
    #     for i in range(len(degress)):
    #         output.write(tzimestamp[i].replace(".", ",") + "\t" + degress[i].replace(".", ",") + "\n")
    #
    #
    # contents = codecs.open('modelOut.txt', encoding='utf-8').read()
    # # print (contents)
    # with open("modelOutComma.txt", 'w') as f2:
    #     f2.write(contents.replace('.', ',').replace("   0,0000000e+00   ", '	').replace("   ", '').replace('e', 'E'))
    #
    # f2.close()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
