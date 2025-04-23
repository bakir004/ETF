
const maxNonAdjacentSum = (array) => {
    if(array.length === 0) return 0;
    if(array.length === 1) return array[0];
    return Math.max(array[0] + maxNonAdjacentSum(array.slice(2)), 
        maxNonAdjacentSum(array.slice(1)))
}

console.log(maxNonAdjacentSum([75,105,120,75,90,135]))
