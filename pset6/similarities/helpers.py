from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""

    a_lines = set(a.split("\n"));
    b_lines = set(b.split("\n"));
    return list(a_lines.intersection(b_lines));



def sentences(a, b):
    """Return sentences in both a and b"""
    a_sentences = set(sent_tokenize(a));
    b_sentences = set(sent_tokenize(b));
    return list(a_sentences.intersection(b_sentences));


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    a_sub = set()
    for i in range(0, len(a)-n+1):
        a_sub.add(a[i:i+n])

    intersection = [ai for ai in a_sub if ai in b]
    return intersection

if __name__ == "__main__":
    print(substrings("foobar", "bar", 3))