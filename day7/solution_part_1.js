const fs = require('fs');
const path = require('path');

const cardStrengths = {
	'A': 14,
	'K': 13,
	'Q': 12,
	'J': 11,
	'T': 10,
	'9': 9,
	'8': 8,
	'7': 7,
	'6': 6,
	'5': 5,
	'4': 4,
	'3': 3,
	'2': 2
}

function getHandType(hand) {
	const cardCounts = {};
	for(let card of hand) {
		cardCounts[card] = (cardCounts[card] || 0) + 1;
	}
	const counts = Object.values(cardCounts).sort((a, b) => b - a);

	if (counts[0] === 5) return 7;
	if (counts[0] === 4) return 6;
	if (counts[0] === 3 && counts[1] === 2) return 5;
	if (counts[0] === 3) return 4;
	if (counts[0] === 2 && counts[1] === 2) return 3;
	if (counts[0] === 2) return 2;
	return 1;
}

function compareHands(hand1, hand2) {
	const type1 = getHandType(hand1);
	const type2 = getHandType(hand2);

	if (type1 !== type2) return type1 - type2;

	for (let i = 0; i < hand1.length; i++) {
		const strength1 = cardStrengths[hand1[i]];
		const strength2 = cardStrengths[hand2[i]];

		if (strength1 !== strength2) return strength1 - strength2;
	}

	return 0;
}

function calculateTotalWinnings(input) {
	const hands = input.trim().split('\n')
		.map(line => {
			const [hand, bid] = line.split(' ');
			return { hand, bid: parseInt(bid) };
		});

	hands.sort((a,b) => compareHands(a.hand, b.hand));

	return hands.reduce((total, hand, index) => {
		return total + hand.bid * (index + 1);
	}, 0)
}

try {
	const inputPath = path.join(__dirname, 'input.txt');
	const input = fs.readFileSync(inputPath, 'utf8');
	const totalWinnings = calculateTotalWinnings(input);
	console.log('Total Winnings: ', totalWinnings);
} catch	(err) {
	console.error(err);
}
