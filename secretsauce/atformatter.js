const fs = require('fs');

function extractSubstring(inputString) {
    const regex = /(?:[^"]*"){5}([^"]*)"/;
    const match = inputString.match(regex);
    if (match && match[1])
        return match[1];
    return '';
}

fs.readFile('at.txt', 'utf8', (err, data) => {
    if (err) { console.error(err); return; }
    data = extractSubstring(data);
    data = data.replaceAll('&quot;', '"');
    data = data.replaceAll('\\/', '/');
    data = data.replaceAll('&amp;', '&');
    fs.writeFileSync('output.json', data);
    const dataJson = JSON.parse(data);
    const tests = dataJson.tests
    let output = ""
    tests.forEach(test => {
        if(test.tools[2].execute.environment) {
            output += test.name + (test.options ? ": Skriven" : ": Nije skriven") + " - Koristi vasu main() funkciju i stdin ulaz\n\nUlaz:\n\n"
            let stdin = test.tools[2].execute.environment.stdin;
            output += stdin;
        } else {
            output += test.name + (test.options ? ": Skriven" : ": Nije skriven") + "\n";
            output += 'Koristi svoju main() funckiju\n\nEvo kako izgleda program nakon spajanja njihovog i vaseg koda:\n'
            output += '========================================================================\n\n'
            output += '// Vas kod ide ovdje\n\n'
            let codeSnippets = test.tools[0].patch;
            codeSnippets.forEach(snippet => {
                if(snippet.position === 'main')
                    output += "int main() {\n";
                
                output += snippet.code + "\n\n"
                if(snippet.position === 'main')
                    output += "    return 0;\n}\n\n"
                
            })
            output += '========================================================================\n\n'
        }
        let expectedOutputArray = test.tools[2].execute.expect;
        if(expectedOutputArray.length > 1) output += '\nOvaj autotest ima vise validnih izlaza!\n'
        expectedOutputArray.forEach((expectedOutput, i) => output += 'Izlaz' + (expectedOutputArray.length > 1 ? " " + (i+1) : "") + ':\n\n' + expectedOutput + '\n\n')
        output += "########################################################################\n"
        output += "########################################################################\n"
        output += "########################################################################\n\n"
    })
    console.log("Formatirani testovi!")
    fs.writeFileSync('output.txt', output);
});

