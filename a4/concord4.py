import sys, fileinput
import re

class concord:

    def __init__(self, input=None, output=None):
        self.input = input
        self.output = output
        self.__raw_lines__ = []
        self.__ex_words__ = []
        self.__sentences__ = []
        self.__indexed_sentences__ = []
        self.__determine_index_words__()
        self.__create_indexed_lines__()

    # call functions to get inputs
    def __determine_index_words__(self):
        self.__get_input__()
        self.__raw_to_ex_and_sent__()

    # change the raw input to lists of ex wordss and __sentences__ to index
    def __raw_to_ex_and_sent__(self):
        start = self.__raw_lines__.index("''''\n")
        mid = self.__raw_lines__.index("\"\"\"\"\n")
        end = len(self.__raw_lines__)

        for i in range(start+1,mid):
            self.__ex_words__.append(self.__raw_lines__[i].strip())
        for j in range(mid+1,end):
            self.__sentences__.append(self.__raw_lines__[j].split())

    # determine whether input should be read from file or stdin
    def __get_input__(self):
        if self.__is_stdin__():
            self.__get_stdin__()
        else:
            self.__get_file__()

    # case get the text from stdin
    def __get_stdin__(self):
        for line in fileinput.input():
            if 'Exit' == line.rstrip():
                break
            self.__raw_lines__.append(line)
    # case get the text from a file
    def __get_file__(self):
        for line in fileinput.input(files = self.input):
            if 'Exit' == line.rstrip():
                break
            self.__raw_lines__.append(line)

    # writes to the output file 
    def __write_to_output__(self):
        file1 = open(self.output, "w")
        file1.writelines("\n".join([x[0] for x in self.__indexed_sentences__]))
        close()
        #todo open the output and write the indexed lines in there 
        # don't forget to close file!

    # determines if is stdin or file input
    def __is_stdin__(self):
        return self.output == None

    def __right_version__(self):
        if(int(self.__raw_lines__[0]) == 2):
            return True
        else:
            return False

    # creates the indexed lines and sorts then writes if neeeded
    def __create_indexed_lines__(self):
        if(self.__right_version__()):
            for line in self.__sentences__:
                self.__line_to_index__(line)
        
            self.__indexed_sentences__.sort(key =lambda x: x[1])    
        
            if self.__is_stdin__() == False:
                self.__indexed_sentences__.append(("", ""))
                self.__write_to_output__()

    # goes thru the line to see what lines to create        
    def __line_to_index__(self, line):
        for word in line:
            if self.is_ex_word(word):
                continue
            else:
                self.__create_an_indexed_line__(word, line)

    # checks if the index word is in the __ex_words__
    def is_ex_word(self,index_word):
        for word in self.__ex_words__:
            if index_word.lower() == word or index_word == word:
                return True
        return False

    def __create_an_indexed_line__(self, index_word, sentence):
        string_sent = self.__list_to_string__(sentence)
        cap_sent = self.__replace_with_capitalized__(index_word, string_sent)
        new_index = re.search(index_word.upper(), cap_sent)
        centered_line =  self.__create_centered_line__(index_word,
                                        cap_sent, new_index.start())
        
        final_entry = (centered_line, index_word.upper())

        self.__indexed_sentences__.append(final_entry)# temp add to the final thing remember to remove
        

# create the centered line with index_word
    def __create_centered_line__(self,index_word,sentence,index_start):
        center = 30
        space = 10
        start_sentence = center - index_start - space
        cur_set_len = 0
        final_sentence = ""

        # cut the front words off
        cut_sent = self.__cut_front__(start_sentence,sentence)
        #get the length of the front cut sentence + spacing
        cur_set_len = len(cut_sent) +  space
        fully_cut_sent = self.__cut_back__(cut_sent)
        return fully_cut_sent

    def __cut_front__(self, start_sentence, sentence):
        space = 9
        while start_sentence < 0:
            x = re.search("\w+\s", sentence)
            sentence = sentence[len(x.group()):]
            start_sentence += len(x.group())
        final_sentence = " "*(start_sentence) +" "*space
        final_sentence += sentence
        return final_sentence

    def __cut_back__(self, sentence):
        cutoff_point = 61
        cut_sent = sentence
        if len(sentence) >= cutoff_point:
            cut_sent = sentence[:cutoff_point]
            while sentence[cutoff_point-1:cutoff_point] != " ":#can't get index of char
                cut_sent = sentence[:cutoff_point-1]           # at sentence[cutoff_point]?
                cutoff_point = cutoff_point - 1                # so must do that    
            cut_sent = sentence[:cutoff_point-1]
        return cut_sent
        
    # finds and replaces word in a sentence with capitalized word
    def __replace_with_capitalized__(self, index, sentence):
        new_index = index.upper()
        new_sentence = re.sub(r"\b" + index + r"\b", new_index, sentence)
        return new_sentence

    # turns a list into a string
    def __list_to_string__(self, the_list):
        sentence = " ".join(the_list)
        return sentence


    def full_concordance(self):
        return [x[0] for x in self.__indexed_sentences__]

