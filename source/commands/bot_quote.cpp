/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_quotes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mweghofe <mweghofe@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:50:28 by mweghofe          #+#    #+#             */
/*   Updated: 2026/09/12 14:29:23 by mweghofe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "IBot.hpp"
#include "irc.hpp"

#include <vector>
#include <cstdlib>
#include <ctime>

namespace
{

bool initQuotes(std::vector<std::string>& quotes)
{
	std::srand(time(NULL));

	// ---------- English ----------
	quotes.push_back("Nature has placed mankind under the governance of two sovereign masters, pain and pleasure. (Jeremy Bentham, An Introduction to the Principles of Morals and Legislation, Ch. 1, 1789)");
	quotes.push_back("No man's knowledge here can go beyond his experience. (John Locke, An Essay Concerning Human Understanding, Book II, 1689)");
	quotes.push_back("Reason is, and ought only to be the slave of the passions. (David Hume, A Treatise of Human Nature, Book II, Part III, Sect. III, 1739)");

	// ---------- Greek ----------
	quotes.push_back("Ὁ δὲ ἀνεξέταστος βίος οὐ βιωτὸς ἀνθρώπῳ. (Socrates, quoted by Plato, Apology 38a, c. 399 BC) The unexamined life is not worth living for a human being.");
	quotes.push_back("Ποταμοῖσι τοῖσιν αὐτοῖσιν ἐμβαίνουσιν, ἕτερα καὶ ἕτερα ὕδατα ἐπιρρεῖ. (Heraclitus, Fragment DK22B12, c. 500 BC) Ever-new waters flow on those who step into the same rivers.");
	quotes.push_back("Μηκέθ' ὅλως περὶ τοῦ οἷόν τινα εἶναι τὸν ἀγαθὸν ἄνδρα διαλέγεσθαι, ἀλλὰ εἶναι τοιοῦτον. (Marcus Aurelius, Meditations 10.16, c. 170-180 AD) Waste no more time arguing what a good man should be. Be one.");

	// ---------- Latin ----------
	quotes.push_back("Non est ad astra mollis e terris via. (Seneca, Hercules Furens, l. 437, 1st c. AD) There is no easy way from earth to the stars.");
	quotes.push_back("Vivere est cogitare. (Cicero, Tusculanae Disputationes, Book V, 45 BC) To live is to think.");
	quotes.push_back("Si fallor, sum. (St. Augustine, De Civitate Dei, Book XI, Ch. 26, 426 AD) If I am mistaken, I exist.");

	// ---------- Italian ----------
	quotes.push_back("La storia è sempre storia contemporanea. (Benedetto Croce, La storia come pensiero e come azione, 1938) History is always contemporary history.");
	quotes.push_back("Il senso comune è un giudizio senz'alcuna riflessione. (Giambattista Vico, Scienza Nuova, 1725) Common sense is judgment without any reflection.");
	quotes.push_back("Fatti non foste a viver come bruti, ma per seguir virtute e canoscenza. (Dante Alighieri, Inferno, Canto XXVI, vv. 119-120, c. 1314) You were not made to live like brutes, but to follow virtue and knowledge.");

	// ---------- German ----------
	quotes.push_back("Habe Mut, dich deines eigenen Verstandes zu bedienen! (Immanuel Kant, Beantwortung der Frage: Was ist Aufklärung?, 1784) Have the courage to use your own understanding!");
	quotes.push_back("Was mich nicht umbringt, macht mich stärker. (Friedrich Nietzsche, Götzen-Dämmerung, 1888) What does not kill me makes me stronger.");
	quotes.push_back("Was vernünftig ist, das ist wirklich; und was wirklich ist, das ist vernünftig. (G.W.F. Hegel, Grundlinien der Philosophie des Rechts, Preface, 1820) What is rational is actual; and what is actual is rational.");

	// ---------- Chinese ----------
	quotes.push_back("己所不欲，勿施于人。 (Confucius, Analects 15.24, c. 5th century BC) Do not impose on others what you yourself do not desire.");
	quotes.push_back("知人者智，自知者明。 (Laozi, Dao De Jing, Ch. 33, c. 6th century BC) Knowing others is wisdom; knowing yourself is enlightenment.");
	quotes.push_back("吾生也有涯，而知也无涯。 (Zhuangzi, Inner Chapters, \"Yangshengzhu,\" c. 4th century BC) My life has a limit, but knowledge has none.");

	// ---------- Spanish ----------
	quotes.push_back("Yo soy yo y mi circunstancia. (Jose Ortega y Gasset, Meditaciones del Quijote, 1914) I am I and my circumstance.");
	quotes.push_back("Lo bueno, si breve, dos veces bueno. (Baltasar Gracian, Oraculo manual y arte de prudencia, 1647) What is good, if brief, is twice as good.");
	quotes.push_back("Piensa el sentimiento, siente el pensamiento. (Miguel de Unamuno, Del sentimiento tragico de la vida, 1913) Feel the thought, think the feeling.");

	// ---------- Japanese ----------
	quotes.push_back("自己をならふといふは、自己をわするるなり。 (Dogen, Shobogenzo, \"Genjokoan,\" 1233) To study the self is to forget the self.");
	quotes.push_back("武士道といふは、死ぬ事と見付けたり。 (Yamamoto Tsunetomo, Hagakure, c. 1716) The way of the samurai is found in death.");
	quotes.push_back("千日の稽古を鍛とし、万日の稽古を錬とす。 (Miyamoto Musashi, The Book of Five Rings, \"Book of the Void,\" c. 1645) A thousand days of training is called forging; ten thousand days of training is called refining.");

	// ---------- Arabic ----------
	quotes.push_back("الفلسفة علم الأشياء بحقائقها بقدر طاقة الإنسان. (Al-Kindi, On First Philosophy, 9th century) Philosophy is the knowledge of the true nature of things, within the limits of human capacity.");
	quotes.push_back("الحكمة صاحبة الشريعة والأخت الرضيعة لها. (Ibn Rushd / Averroes, The Decisive Treatise, 12th century) Wisdom is the companion of the Sacred Law and its milk-sister.");
	quotes.push_back("الناس عبيد لما عرفوا وأعداء لما جهلوا. (Al-Ghazali, Ihya Ulum al-Din, 11th century) People are slaves to what they know and enemies to what they don't know.");

	// ---------- Sanskrit ----------
	quotes.push_back("कर्मण्येवाधिकारस्ते मा फलेषु कदाचन। (Bhagavad Gita 2.47, c. 2nd century BC - 2nd century AD) You have a right to perform your duty, but never to the fruits of your actions.");
	quotes.push_back("सत्यमेव जयते। (Mundaka Upanishad 3.1.6, c. 1st millennium BC) Truth alone triumphs.");
	quotes.push_back("योगश्चित्तवृत्तिनिरोधः। (Patanjali, Yoga Sutras 1.2, c. 2nd century BC - 4th century AD) Yoga is the cessation of the fluctuations of the mind.");

	return (true);
}

} // end of namespace

void bot_quote(IBot& bot, const Message& msg, std::vector<std::string>& botcmds)
{
	(void) botcmds;
	if (msg.params.empty())
		return ;
	static bool init = false;
	static std::vector<std::string> quotes;
	if (!init)
		init = initQuotes(quotes);
	int i = std::rand() % 30;
	std::string target;
	if (msg.params[0] == BOT_NAME)
		target = msg.prefix.substr(0, msg.prefix.find('!'));
	else
		target = msg.params[0];
	bot.sendMessage("PRIVMSG " + target + " :" + quotes[i] + CRLF);
}
