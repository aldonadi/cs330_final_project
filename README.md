# CS330 - Computational Graphics and Visualization
SNHU CS330 Module 8 Journal

## How do I approach designing software?

The simulated financial planning firm Artemis Financial engaged my company to perform a white-box vulnerability assessment on their proprietary RESTful API. My task was to identify, categorize, and prioritize vulnerabilities and to provide clear and actionable insights for remediation.

## How do I approach developing programs?

I did not merely run their code base and dependency list through an automated scanning tool, but I also analyzed the reported findings and wrote an organized and readable report. I sought to guide a lay reader through the maze of myriad definitions and technical terms in a way that is instructional and easy to understand. I categorized the findings and assigned relative importance levels to the vulnerabilities based on their potential impact and possibility of being exploited. I also provided a color-coded legend to explain each rating.

Secure coding is important because the threat of exploited vulnerabilities is an ever-increasing problem in terms of frequency of occurrence and catastrophic cost. Following security best practices leverages the ever-expanding experience of the cybersecurity industry to minimize or even prevent costly and time-consuming hacks, outages, reputation hits, and potentially regulatory fines or lawsuits. While an application might still have undiscovered vulnerabilities, taking a “defense in depth” approach to establish security at as many levels as is practicable can mitigate or even halt the spread of an infection or intrusion. Just as the consequences of a security breach directly affect the value of the company, so also does the incorporation of safe security practices.

## How can computer science help me in reaching my goals?

Proving reported vulnerabilities as false-positives was the most challenging aspect of the assessment. Nearly every single dependency in the sample project flagged for vulnerabilities. I had to conduct exhaustive research to confidently assess a reported vulnerability as safe. I consulted the NVD and MITRE databases, Google searches, bug tracker threads, assembling all of the web pages and documents I could find. I then read and assessed each document, building a list of indicators I needed to evaluate the code base for. These indicators were things like the exact versions that were affected and which functions or configurations triggered the vulnerability. I then inspected the code base and evaluated each indicator. Only if I could conclusively prove that each and every indicator went in the direction of false positive could I finally mark the dependency as safe. For several dependencies, there was at least one reported vulnerability that I had high confidence was a false positive, but I could not conclusively prove one indicator. Therefore, I had to mark the dependency as possibly unsafe. This was a time-consuming experience, but it is important so that my results to the customer are accurate and actionable.
