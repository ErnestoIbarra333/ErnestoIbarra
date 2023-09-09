import React from "react";
import Card from "./Card";

function getRandomArbitrary(min, max) {
    return Math.random() * (max - min) + min;
}

const CardList = ({robots}) => {
    return (
        <div> 
            {
            robots.map((user,i) => {
                //let generated = getRandomArbitrary(0,100);
                return (
                    <Card 
                        key= {i} 
                        id={robots[i].id} 
                        name={robots[i].name} 
                        email={robots[i].email}
                    />
                );
                })
            }
        </div>
    );
}
export default CardList;