//
//  OnDiet.swift
//  Best Lifter
//
//  Created by νΌμμ on 2021/04/21.
//

import SwiftUI

struct OnDiet: View {
    @ObservedObject var onDietViewModel = OnDietViewModel()
    @ObservedObject var dietViewModel = DietViewModel()
    @Environment(\.presentationMode) var presentationMode: Binding<PresentationMode>
    @State var isAddDiet: Bool = false
    @State var isSubView: Bool = false
    
    var body: some View {
        GeometryReader{ (geometry) in
            ZStack {
                VStack {
                    HStack {
                        Spacer()
                        Button(action: {
                            self.presentationMode.wrappedValue.dismiss()
                            
                            dietViewModel.userCalorieTaked += self.onDietViewModel.calorie
                            dietViewModel.userCarbTaked += self.onDietViewModel.carb
                            dietViewModel.userProteinTaked += self.onDietViewModel.protein
                            dietViewModel.userFatTaked += self.onDietViewModel.fat
                            
                            self.onDietViewModel.setNutritionToZero()
                            self.dietViewModel.setNutritionCGFloat()
                        }, label: {
                            Text("Cancel")
                                .foregroundColor(Color.black)
                                .font(.system(size: 20, weight: .regular, design: .default))
                        })
                        .frame(width: 80, height: 30, alignment: .center)
                        .overlay(
                            RoundedRectangle(cornerRadius: 10.0)
                                .stroke(lineWidth: 2)
                        )
                        .padding(.trailing, 20)
                    }
                    
                    Text("Diet")
                        .padding(.top, 20)
                        .padding(.leading, 20)
                        .frame(width: geometry.size.width, alignment: .leading)
                        .font(.largeTitle)
                    
                    Divider()
                        .background(Color.black)
                        .padding()
                    
                    ScrollView {
                        VStack {
                            Text("μ λ³΄")
                                .padding(.top, 20)
                                .padding(.leading, 20)
                                .frame(width: geometry.size.width, alignment: .leading)
                                .font(.title)
                            Divider()
                            Text("μΉΌλ‘λ¦¬ : \(String(format: "%d", onDietViewModel.meals.calories))")
                                .padding(.leading, 20)
                                .padding(.top, 10)
                                .frame(width: geometry.size.width, alignment: .leading)
                                .font(.title2)
                            
                            Text("νμνλ¬Ό : \(String(format: "%d", onDietViewModel.meals.carbs))")
                                .padding(.leading, 20)
                                .frame(width: geometry.size.width, alignment: .leading)
                                .font(.title2)
                            
                            Text("λ¨λ°±μ§ : \(String(format: "%d", onDietViewModel.meals.proteins))")
                                .padding(.leading, 20)
                                .frame(width: geometry.size.width, alignment: .leading)
                                .font(.title2)
                            
                            Text("μ§λ°© : \(String(format: "%d", onDietViewModel.meals.fats))")
                                .padding(.leading, 20)
                                .frame(width: geometry.size.width, alignment: .leading)
                                .font(.title2)
                        }
                        Divider()
                        
                        ForEach(onDietViewModel.meals.meals, id: \.mealTime) { meal in
                            MealSection(mealTime: meal.mealTime, foods: meal.foods)
                            Divider()
                        }
                    }
                    
                    Spacer()
                    
                    Divider()
                        .background(Color.black)
                        .padding()
                    
                    Button(action: {
                        withAnimation(.easeOut(duration: 0.1)) {
                            self.isAddDiet.toggle()
                        }
                    }, label: {
                        Text("μλ¨ μΆκ°")
                            .frame(width: geometry.size.width - 40, height: 30)
                            .overlay(
                                RoundedRectangle(cornerRadius: 10.0)
                                    .stroke(lineWidth: 2)
                            )
                            .foregroundColor(Color.black)
                    })
                }
                
                if isAddDiet {
                    addDietView
                        .transition(.scale)
                }
            }
        }
    }
    
    var addDietView: some View {
        VStack(spacing: 20) {
            ForEach(onDietViewModel.dietButtons, id: \.self) { dietButton in
                Button(action: {
                    if dietButton == "μ·¨μ" {
                        self.isAddDiet.toggle()
                    } else {
                        self.onDietViewModel.selectedButton = dietButton
                        self.isSubView.toggle()
                    }
                }, label: {
                    Text(dietButton)
                        .font(.system(size: 20, weight: .regular, design: .default))
                        .frame(width: UIScreen.main.bounds.width - 60, height: 50)
                        .overlay(
                            RoundedRectangle(cornerRadius: 10.0)
                                .stroke(lineWidth: 2)
                        )
                        .foregroundColor(Color.black)
                })
                .fullScreenCover(isPresented: $isSubView, onDismiss: {
                    self.dietViewModel.userCalorieTaked += onDietViewModel.calorie
                    self.dietViewModel.userCarbTaked += onDietViewModel.carb
                    self.dietViewModel.userProteinTaked += onDietViewModel.protein
                    self.dietViewModel.userFatTaked += onDietViewModel.fat
                    self.dietViewModel.setNutritionCGFloat()
                    self.isAddDiet.toggle()
                },content: {
                    onDietViewModel.checkSelectedButton(isSubView: $isSubView, isAddDiet: $isAddDiet, onDietViewModel: onDietViewModel, dietViewModel: dietViewModel)
                })
            }
        }
        .padding()
        .background(Color.white.cornerRadius(10.0))
        .overlay(
            RoundedRectangle(cornerRadius: 10.0)
                .stroke(lineWidth: 2)
        )
    }
}

struct MealSection: View {
    var mealTime: String
    var foods: [Food]
    
    init(mealTime: String, foods: [Food]) {
        self.mealTime = mealTime
        self.foods = foods
    }
    
    var body: some View {
        Text("\(mealTime)")
            .font(.system(.title))
            .padding(.leading, 20)
            .padding(.bottom, 10)
            .frame(width: UIScreen.main.bounds.width, alignment: .topLeading)
        
        ForEach(foods, id:\.name) { food in
            FoodSection(food: food)
                .frame(width: UIScreen.main.bounds.width - 40, alignment: .center)
                .overlay(
                    RoundedRectangle(cornerRadius: 10)
                        .stroke(lineWidth: 2))
        }
    }
}

struct FoodSection: View {
    var food: Food
    
    init(food: Food) {
        self.food = food
    }
    
    var body: some View {
        VStack {
            Text(food.name)
                .font(.title2)
                .padding(.vertical, 5)
            Divider()
            
            VStack(alignment: .leading) {
                Text(String(format: "μΉΌλ‘λ¦¬: %.2f", food.calorie))
                Text(String(format: "νμνλ¬Ό: %.2f", food.carb))
                Text(String(format: "λ¨λ°±μ§: %.2f", food.protein))
                Text(String(format: "μ§λ°©: %.2f", food.fat))
            }
            .padding(.vertical, 5)
            .frame(width: UIScreen.main.bounds.width, alignment: .leading)
            .padding(.leading, 60)
        }
    }
}

struct OnDiet_Previews: PreviewProvider {
    static var previews: some View {
        OnDiet()
//        TransactionBarChartView(entries: Transaction.dataEntriesForYear(2021, transactions: Transaction.allTrasactions))
    }
}
