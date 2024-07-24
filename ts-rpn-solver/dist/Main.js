"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const RpnSolver_1 = require("./solver/RpnSolver");
const Tokenizer_1 = require("./solver/Tokenizer");
const TokensToRpn_1 = require("./solver/TokensToRpn");
function main() {
    const args = process.argv.slice(2);
    let expression;
    if (args.length === 0) {
        // Use a sample expression if no arguments are provided
        expression = "3 + 4 * 2 / (1 - 5)^2^1";
        console.log("Using sample expression:", expression);
    }
    else if (args.length === 1) {
        // Use the provided expression
        expression = args[0];
        console.log("Using provided expression:", expression);
    }
    else {
        // Display usage message for incorrect number of arguments
        console.log("Usage: node main.js [expression]");
        console.log("If no expression is provided, a sample expression will be used.");
        return;
    }
    try {
        // Step 1: Tokenize
        console.log("\nStep 1: Tokenizing");
        const tokens = (0, Tokenizer_1.tokenize)(expression);
        console.log("Tokens:", tokens);
        // Step 2: Convert to RPN
        console.log("\nStep 2: Converting to RPN");
        const rpnTokens = (0, TokensToRpn_1.tokensToRpn)(tokens);
        console.log("RPN Tokens:", rpnTokens);
        // Step 3: Solve RPN
        console.log("\nStep 3: Solving RPN");
        const result = (0, RpnSolver_1.solveRpn)(rpnTokens);
        console.log("Result:", result);
    }
    catch (error) {
        console.error("An error occurred:", error.message);
    }
}
main();
