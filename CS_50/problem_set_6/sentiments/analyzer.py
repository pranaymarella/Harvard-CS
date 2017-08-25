import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positive_words = set()
        self.negative_words = set()
        # TODO
        self.load(positives, negatives)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        # TODO
        if text.lower() in self.positive_words:
            return 1
        elif text.lower() in self.negative_words:
            return -1
        else:
            return 0
    
    def load(self, positives, negatives):
        file_positive = open(positives, 'r')
        for line in file_positive:
            if not line.startswith(';'):
                self.positive_words.add(line.rstrip('\n'))
        file_positive.close()
        
        file_negative = open(negatives, 'r')
        for line in file_negative:
            if not line.startswith(';'):
                self.negative_words.add(line.rstrip('\n'))
        file_negative.close()
        return True